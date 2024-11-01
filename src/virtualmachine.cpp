#include "virtualmachine.h"

#include <capstone/capstone.h>

#include <QFile>

#include <QDebug>

#define IMAGE_DOS_SIGNATURE 0x5A4D
#define IMAGE_NT_SIGNATURE  0x00004550

#define STACK_SIZE 0x10000 // 64 KB stack size
#define STACK_ADDR 0x1000000 // Example stack address

#define IMAGE_DIRECTORY_ENTRY_EXPORT 0
#define IMAGE_DIRECTORY_ENTRY_IMPORT 1

#define IMAGE_SCN_MEM_EXECUTE 0x20000000

#pragma pack(push, 1)
typedef struct {
    uint16_t e_magic;
    uint16_t e_cblp;
    uint16_t e_cp;
    uint16_t e_crlc;
    uint16_t e_cparhdr;
    uint16_t e_minalloc;
    uint16_t e_maxalloc;
    uint16_t e_ss;
    uint16_t e_sp;
    uint16_t e_csum;
    uint16_t e_ip;
    uint16_t e_cs;
    uint16_t e_lfarlc;
    uint16_t e_ovno;
    uint16_t e_res[4];
    uint16_t e_oemid;
    uint16_t e_oeminfo;
    uint16_t e_res2[10];
    uint32_t e_lfanew;
} IMAGE_DOS_HEADER;

typedef struct {
    uint32_t Signature;
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
} IMAGE_FILE_HEADER;

typedef struct {
    uint32_t VirtualAddress;
    uint32_t Size;
} IMAGE_DATA_DIRECTORY;

typedef struct {
    uint16_t  Magic;
    uint8_t   MajorLinkerVersion;
    uint8_t   MinorLinkerVersion;
    uint32_t  SizeOfCode;
    uint32_t  SizeOfInitializedData;
    uint32_t  SizeOfUninitializedData;
    uint32_t  AddressOfEntryPoint;
    uint32_t  BaseOfCode;
    uint32_t  BaseOfData;
    uint32_t  ImageBase;
    uint32_t  SectionAlignment;
    uint32_t  FileAlignment;
    uint16_t  MajorOperatingSystemVersion;
    uint16_t  MinorOperatingSystemVersion;
    uint16_t  MajorImageVersion;
    uint16_t  MinorImageVersion;
    uint16_t  MajorSubsystemVersion;
    uint16_t  MinorSubsystemVersion;
    uint32_t  Win32VersionValue;
    uint32_t  SizeOfImage;
    uint32_t  SizeOfHeaders;
    uint32_t  CheckSum;
    uint16_t  Subsystem;
    uint16_t  DllCharacteristics;
    uint32_t  SizeOfStackReserve;
    uint32_t  SizeOfStackCommit;
    uint32_t  SizeOfHeapReserve;
    uint32_t  SizeOfHeapCommit;
    uint32_t  LoaderFlags;
    uint32_t  NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY  DataDirectory[16];
} IMAGE_OPTIONAL_HEADER32;

typedef struct {
    uint8_t  Name[8];
    uint32_t VirtualSize;
    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToRelocations;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    uint32_t Characteristics;
} IMAGE_SECTION_HEADER;

typedef struct {
    uint32_t Characteristics;
    uint32_t TimeDateStamp;
    uint16_t MajorVersion;
    uint16_t MinorVersion;
    uint32_t Name;
    uint32_t Base;
    uint32_t NumberOfFunctions;
    uint32_t NumberOfNames;
    uint32_t AddressOfFunctions;
    uint32_t AddressOfNames;
    uint32_t AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY;

typedef struct {
    uint32_t OriginalFirstThunk;
    uint32_t TimeDateStamp;
    uint32_t ForwarderChain;
    uint32_t Name;
    uint32_t FirstThunk;
} IMAGE_IMPORT_DESCRIPTOR;

#pragma pack(pop)

void VirtualMachine::dumpStack()
{
    uint32_t tmp;
    uint32_t stackPtr;

    uc_reg_read(m_uc, UC_X86_REG_ESP, &stackPtr);

    for(int i = 0; i < 16; i++)
    {
        uc_mem_read(m_uc, stackPtr, &tmp, sizeof(uint32_t));
        qDebug() << "stack " << Qt::hex << stackPtr << ":" << Qt::hex << tmp;

        stackPtr += sizeof(uint32_t);
    }
}

bool VirtualMachine::hook_code(uc_engine *uc, uint64_t address, uint32_t size, void *user_data)
{
    VirtualMachine *machine = reinterpret_cast<VirtualMachine*>(user_data);

    auto importedSymbols = machine->importedSymbols();

    uint8_t code[size];
    uc_mem_read(uc, address, code, size);

//    if(address == 0x10001066)
//    {
//        uint8_t al;
//        uc_reg_read(uc, UC_X86_REG_AL, &al);

//        qDebug() << "al value" << al;
//    }

//    if(address == 0x100032a5)
//    {
//        uint32_t esi;
//        uc_reg_read(uc, UC_X86_REG_ESI, &esi);

//        qDebug() << "esi" << Qt::hex << esi;
//    }

//    if(address == 0x1000cb51)
//    {
//        float wtf;
//        uc_mem_read(uc, 0x10023200, &wtf, sizeof(float));

//        qDebug() << "wtf" << wtf;
//    }

    if(machine->debug())
    {
        QDebug deb = qDebug();
        deb << Qt::hex << address;

        csh handle;
        cs_insn *insn;
        cs_err err = cs_open(CS_ARCH_X86, CS_MODE_32, &handle);
        if (err != CS_ERR_OK)
        {
            qCritical() << "Failed to open Capstone" << err;
            return false;
        }

        QStringList opcode;
        for(auto i = 0; i < size; i++)
        {
            opcode << QString::number(code[i], 16);
        }

        deb << opcode.join(" ");

        size_t count = cs_disasm(handle, code, sizeof(code), address, 1, &insn);
        if (count > 0)
        {
            deb << insn[0].mnemonic << insn[0].op_str;

            cs_free(insn, count);
        }
        else
        {
            qCritical() << "Failed to disassemble instruction at address" << address;
        }

        cs_close(&handle);

//        uint32_t eax;
//        uc_reg_read(uc, UC_X86_REG_EAX, &eax);

//        qDebug() << "eax" << eax;
    }

    // test for call or jump instruction
    if(code[0] == 0xff && (((0x38 & code[1]) >> 3) == 4 || ((0x38 & code[1]) >> 3) == 2))
    {
        uint32_t addr = code[2] | (code[3] << 8) | (code[4] << 16) | (code[5] << 24);

        if(importedSymbols.contains(addr))
        {
            auto fun = importedSymbols[addr];

            uint32_t esp;
            uc_reg_read(uc, UC_X86_REG_ESP, &esp); // Read current stack pointer value

            bool call = ((0x38 & code[1]) >> 3) == 2;

            if(call)
            {
                if(machine->debug()) qDebug() << "call to dll " << (QString) fun;
                // we need to save stack ptr

                uint32_t eip;
                uc_reg_read(uc, UC_X86_REG_EIP, &eip);

                eip += size; // point to next instruction

                esp -= sizeof(uint32_t);
                uc_mem_write(uc, esp, &eip, sizeof(uint32_t));
            }
            else
                if(machine->debug()) qDebug() << "jump to dll " << (QString) fun;

            if(machine->implementations().contains((QString) fun))
            {
                auto impl = machine->implementations()[fun];

                int off = 0;
                char *data = (char *)malloc(impl->size());
                char *retdata = nullptr;

                for(int i = 0; i < impl->numArgs(); i++)
                {
                    auto arg = impl->arg(i);
                    uc_mem_read(uc, esp + 0x4 + off, data + off, arg);
                    off += arg;
                }

                if(impl->hasReturn())
                {
                    //qDebug() << "impl has return" << impl->returnSize();

                    auto retSize = impl->returnSize();
                    if(retSize > 4)
                    {
                        qCritical() << "return bigger than 4 bytes not implemented";
                    }

                    retdata = (char *) malloc(impl->returnSize());

                    impl->call(machine, data, retdata);

                    uint32_t retEax; //!FIXME is this right ?
                    if(retSize == 1) retEax = *reinterpret_cast<uint8_t*>(retdata);
                    if(retSize == 2) retEax = *reinterpret_cast<uint16_t*>(retdata);
                    if(retSize == 4) retEax = *reinterpret_cast<uint32_t*>(retdata);

                    uc_reg_write(uc, X86_REG_EAX, &retEax);
                }
                else
                {
                    impl->call(machine, data);
                }

                if(retdata)
                    free(retdata);

                free(data);
            }
            else
            {
                qDebug() << "unimplemented function " << (QString) fun << " exiting";

                uc_emu_stop(uc);

                return true;
            }

            uint32_t retaddr;
            uc_mem_read(uc, esp, &retaddr, sizeof(uint32_t));

            esp += sizeof(uint32_t);
            uc_reg_write(uc, UC_X86_REG_ESP, &esp);

            if(machine->debug()) qDebug() << "returning to 0x" << Qt::hex << retaddr;

            uc_reg_write(uc, UC_X86_REG_EIP, &retaddr);

            return true;
        }
    }

    return false;
}

void VirtualMachine::hook_mem_invalid(uc_engine *uc, uc_mem_type type, uint64_t address,
                                      int size, int64_t value, void *user_data)
{
    qDebug() << "Invalid memory access at address" << Qt::hex << address;

    uc_emu_stop(uc);
}

VirtualMachine::~VirtualMachine()
{
    if(m_uc)
        uc_close(m_uc);
}

bool VirtualMachine::init()
{
    uc_err err = uc_open(UC_ARCH_X86, UC_MODE_32, &m_uc);
    if (err != UC_ERR_OK)
    {
        qCritical() << "Failed to initialize Unicorn: " << uc_strerror(err);
        return false;
    }

    uc_hook code_hook;
    uc_hook_add(m_uc, &code_hook, UC_HOOK_CODE, (void*)hook_code, this, 1, 0);

    uc_hook invalid_mem_hook;
    uc_hook_add(m_uc, &invalid_mem_hook, UC_HOOK_MEM_INVALID, (void*)hook_mem_invalid, this, 1, 0);

    return true;
}

bool VirtualMachine::load(QString fileName)
{
    QFile peFile(fileName);
    if (!peFile.open(QIODevice::ReadOnly))
    {
        qCritical() << "Failed to open PE file";
        return false;
    }

    IMAGE_DOS_HEADER dos_header;
    peFile.read((char *)&dos_header, sizeof(IMAGE_DOS_HEADER));
    if (dos_header.e_magic != IMAGE_DOS_SIGNATURE)
    {
        qCritical() << "Not a valid DOS executable";
        return false;
    }

    qDebug() << "PE header " << Qt::hex << dos_header.e_lfanew;

    IMAGE_FILE_HEADER file_header;
    peFile.seek(dos_header.e_lfanew);
    peFile.read((char *)&file_header, sizeof(IMAGE_FILE_HEADER));

    if(file_header.Signature != IMAGE_NT_SIGNATURE)
    {
        qCritical() << "not a valid PE file";
        return false;
    }

    uint32_t optional_header_offset = peFile.pos();
    uint32_t section_header_offset = optional_header_offset + file_header.SizeOfOptionalHeader;

    IMAGE_OPTIONAL_HEADER32 optional_header;
    peFile.read((char *)&optional_header, sizeof(IMAGE_OPTIONAL_HEADER32));

    qDebug() << "image base" << Qt::hex << optional_header.ImageBase;
    qDebug() << "code base" << Qt::hex << optional_header.BaseOfCode;
    qDebug() << "entry point" << Qt::hex << optional_header.AddressOfEntryPoint;
    qDebug() << "optional header:" << file_header.SizeOfOptionalHeader;
    qDebug() << "sections:" << file_header.NumberOfSections;

    uint32_t peHeaderSize = section_header_offset + (file_header.NumberOfSections * sizeof(IMAGE_SECTION_HEADER));
    uint8_t *peHeaderData = (uint8_t*) malloc(peHeaderSize);

    uint32_t peHeaderSizeAligned = (peHeaderSize + 4095) & (~4095);
    uc_mem_map(m_uc, optional_header.ImageBase, peHeaderSizeAligned, UC_PROT_ALL);

    peFile.seek(0);
    peFile.read((char*)peHeaderData, peHeaderSize);
    uc_mem_write(m_uc, optional_header.ImageBase, peHeaderData, peHeaderSize);
    free(peHeaderData);

    IMAGE_SECTION_HEADER section_header;
    for (int i = 0; i < file_header.NumberOfSections; ++i)
    {
        peFile.seek(section_header_offset + i*sizeof(IMAGE_SECTION_HEADER));
        peFile.read((char *)&section_header, sizeof(IMAGE_SECTION_HEADER));

        qDebug() << "section" << (char*)section_header.Name << i
                 << "addr" << Qt::hex << section_header.VirtualAddress
                 << "siz" << Qt::hex << section_header.VirtualSize
                 << "phys" << Qt::hex << section_header.PointerToRawData;

        uint32_t flags = UC_PROT_READ | UC_PROT_WRITE;
        if(section_header.Characteristics & IMAGE_SCN_MEM_EXECUTE)
        {
            flags |= UC_PROT_EXEC;
        }

        uint32_t vaddr = optional_header.ImageBase + section_header.VirtualAddress;
        uint32_t vsiz = (section_header.VirtualSize + 4095) & (~4095);

        uc_mem_map(m_uc, vaddr, vsiz, flags);

        uint8_t* buffer = (uint8_t*) malloc(section_header.SizeOfRawData);
        peFile.seek(section_header.PointerToRawData);
        peFile.read((char *)buffer, section_header.SizeOfRawData);
        uc_mem_write(m_uc, vaddr, buffer, section_header.SizeOfRawData);
        free(buffer);
    }

    peFile.close();

    IMAGE_DATA_DIRECTORY symbol_table_dir = optional_header.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    if (symbol_table_dir.VirtualAddress != 0 && symbol_table_dir.Size != 0)
    {
        IMAGE_EXPORT_DIRECTORY export_dir;

        uc_mem_read(m_uc, optional_header.ImageBase + symbol_table_dir.VirtualAddress,
                    &export_dir, sizeof(IMAGE_EXPORT_DIRECTORY));

        char dll_name[256];
        uc_mem_read(m_uc, optional_header.ImageBase + export_dir.Name, dll_name, sizeof(dll_name));

        qDebug() << "DLL Name:" << dll_name;

        uint32_t* function_names = (uint32_t*)malloc(export_dir.NumberOfNames * sizeof(uint32_t));
        uc_mem_read(m_uc, optional_header.ImageBase + export_dir.AddressOfNames,
                    function_names, sizeof(uint32_t)*export_dir.NumberOfNames);

        uint16_t* function_ordinals = (uint16_t*)malloc(export_dir.NumberOfFunctions * sizeof(uint16_t));
        uc_mem_read(m_uc, optional_header.ImageBase + export_dir.AddressOfNameOrdinals,
                    function_ordinals, sizeof(uint16_t) * export_dir.NumberOfFunctions);

        uint32_t* function_addresses = (uint32_t*)malloc(export_dir.NumberOfFunctions * sizeof(uint32_t));
        uc_mem_read(m_uc, optional_header.ImageBase + export_dir.AddressOfFunctions,
                    function_addresses, sizeof(uint32_t) * export_dir.NumberOfFunctions);

        for (uint32_t i = 0; i < export_dir.NumberOfFunctions; ++i)
        {
            char function_name[256];
            uc_mem_read(m_uc, optional_header.ImageBase + function_names[i], function_name, sizeof(function_name));

            int ord = function_ordinals[i];

            qDebug() << "Function Name:" << function_name
                     << "Ordinal:" << function_ordinals[i]
                     << "Address:" << Qt::hex << function_addresses[ord];

            m_functionMap[function_name] = optional_header.ImageBase + function_addresses[ord];
        }

        free(function_names);
        free(function_ordinals);
        free(function_addresses);
    }

    IMAGE_DATA_DIRECTORY import_dir = optional_header.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (import_dir.VirtualAddress == 0 || import_dir.Size == 0)
    {
        printf("No import table found\n");
        return -1;
    }

    uint32_t import_dir_offset = optional_header.ImageBase + import_dir.VirtualAddress;

    qDebug() << "import table " << Qt::hex << import_dir_offset;

    while (1)
    {
        IMAGE_IMPORT_DESCRIPTOR import_desc;
        uc_mem_read(m_uc, import_dir_offset, &import_desc, sizeof(IMAGE_IMPORT_DESCRIPTOR));

        if (import_desc.Name == 0) // end
            break;

        char dll_name[256];
        uc_mem_read(m_uc, optional_header.ImageBase + import_desc.Name, dll_name, sizeof(dll_name));

        uint32_t thunk_data_ptr = optional_header.ImageBase + import_desc.FirstThunk;

        while (1)
        {
            uint32_t thunk_data;

            uc_mem_read(m_uc, thunk_data_ptr, &thunk_data, sizeof(thunk_data));

            if (thunk_data == 0) // end
                break;

            if (thunk_data & 0x80000000)
            {
                qDebug() << "DLL:" << dll_name << "Ordinal: " << (thunk_data & ~0x80000000);
            }
            else
            {
                char symbol_name[256];
                uc_mem_read(m_uc, optional_header.ImageBase + thunk_data + 2, symbol_name, sizeof(symbol_name));

                qDebug() << "DLL:" << dll_name
                         << "Symbol:" << symbol_name
                         << "addr" << Qt::hex << thunk_data_ptr;

                symbolImport imp;
                imp.symbol = QString(symbol_name);
                imp.module = QString(dll_name).toLower();

                m_importedSymbols[thunk_data_ptr] = imp;
            }

            thunk_data_ptr += sizeof(uint32_t);
        }

        import_dir_offset += sizeof(IMAGE_IMPORT_DESCRIPTOR); // Next import descriptor
    }

    // map msvcrt iob shit
    for(auto addr : m_importedSymbols.keys())
    {
        if ((QString) m_importedSymbols[addr] == "msvcrt.dll:_iob")
        {
            uint32_t iobAddr = 0x1000;

            uc_mem_map(m_uc, iobAddr, 4096, UC_PROT_READ | UC_PROT_WRITE);
            uc_mem_write(m_uc, addr, &iobAddr, sizeof(uint32_t));

            qDebug() << "patched msvcrt iob";
        }
    }

    uc_mem_map(m_uc, 0x0, 4096, UC_PROT_READ);    // map thread environment block shit

    uc_mem_map(m_uc, STACK_ADDR - STACK_SIZE, STACK_SIZE, UC_PROT_ALL);
    uint32_t esp_value = STACK_ADDR - 32; //!FIXME stack padding

    uc_reg_write(m_uc, UC_X86_REG_ESP, &esp_value); // set some stack

    uint32_t entry_point_address = optional_header.ImageBase + optional_header.AddressOfEntryPoint;

    qDebug() << "starting machine: entry";

    uint32_t function_address = entry_point_address;

    uc_err err = uc_emu_start(m_uc, function_address, 0, 0, 0);
    if (err != UC_ERR_OK) {
        printf("Error while emulating: %s\n", uc_strerror(err));
    }

    return true;
}

uint32_t VirtualMachine::push(void *data, int size)
{
    int asiz = alignTo4Byte(size);

    uint32_t esp; // copy args to machine stack
    uc_reg_read(m_uc, UC_X86_REG_ESP, &esp);

    esp -= asiz;
    uc_mem_write(m_uc, esp, data, size);

    uc_reg_write(m_uc, UC_X86_REG_ESP, &esp);

    return esp;
}

void VirtualMachine::callF(QString function, QVector<uint8_t> args, const char *data)
{
    if(!m_functionMap.contains(function))
    {
        qCritical() << "cant find function" << function;
        return;
    }

    uint32_t esp; // copy args to machine stack
    uc_reg_read(m_uc, UC_X86_REG_ESP, &esp);

    if(debug()) qDebug() << "call" << function;

    int sum = 0;
    sum = std::accumulate(args.begin(), args.end(), sum);

    esp -= sum; // allocate stack memory for all arguments

    uint8_t off = 0;
    for(uint8_t argSize : args)
    {
        uc_mem_write(m_uc, esp + off, data + off, argSize);

        off += argSize;
    }

    esp -= sizeof(uint32_t); // simulate return address
    uint32_t z = 0xffffffff;
    uc_mem_write(m_uc, esp, &z, sizeof(uint32_t));

    uc_reg_write(m_uc, UC_X86_REG_ESP, &esp);

    uint32_t function_address = m_functionMap[function];
    uc_err err = uc_emu_start(m_uc, function_address, 0xffffffff, 0, 0);
    if (err != UC_ERR_OK)
    {
        printf("Error while emulating: %s\n", uc_strerror(err));
    }
}

void VirtualMachine::read(uint32_t addr, void *buff, size_t size)
{
    uc_mem_read(m_uc, addr, buff, size);
}

void VirtualMachine::write(uint32_t addr, void *buff, size_t size)
{
    uc_mem_write(m_uc, addr, buff, size);
}

void VirtualMachine::reg(int regid, void *val)
{
    uc_err err = uc_reg_read(m_uc, regid, val);
    if (err != UC_ERR_OK)
    {
        printf("VirtualMachine::reg: %s\n", uc_strerror(err));
    }
}

void VirtualMachine::setReg(int regid, void *val)
{
    uc_err err = uc_reg_write(m_uc, regid, val);
    if (err != UC_ERR_OK)
    {
        printf("VirtualMachine::setReg: %s\n", uc_strerror(err));
    }
}


