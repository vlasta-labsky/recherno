#include "api.h"
#include "virtualmachine.h"

// 0: manual
// 1: auto
// 2: manual control valve value
// 3: level setpoint
// 4: startup valve
// 5: main valve
// 6: 3 element control off
// 7: 3 element control on
// 8: reactor drain manual
// 9: reactor drain auto
// 10: reactor drain level setpoint
// 11: reactor drain valve increments
// 12, 13: offline cooling control inc
void Chrnob32API::SETFED(float val, uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t valPtr = m_machine->push(&val, sizeof(val));
    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("SETFED", valPtr, idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// 0: startup valve value
// 1: feed water flow
// 3,4,5: feed pumps
// 6: drain flow
// 7: feed level setpoint
// 8: startup valve (1) main valve (2)
// 9: 3 element control off (1) on (2)
// 10: main valve value
// 11: feed auto (0) manual (1)
// 12: drain auto (0) manual (1)
// 13: reactor drain setpoint
// 14: reactor drain valve
float Chrnob32API::GETFED(uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("GETFED", idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

// val off (0) on (1)
// 0, 1, 2, 3, 4, 5 - main recirculation pumps
// 6, 7, 8 - feed pumps
// 9, 10 - condenser circulating pumps
// 11, 12, 13 - condensate pumps
// 14, 15 - offline cooling pumps
// 16, 17 - offline cooling inlet
// 18, 19 - offline cooling outlet
// 20, 21 - ecc pumps
// 22 - diesel pump
// 23 - ecc cooling valve: 0 (close) 1 (open) -1 (auto)
void Chrnob32API::SETPMP(uint16_t id, int16_t val)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));
    uint32_t valPtr = m_machine->push(&val, sizeof(val));

    m_machine->call("SETPMP", idPtr, valPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// get pump state uint16_t[6], flow float[2] loop1 and loop2
void Chrnob32API::GETPMP(uint16_t *pumps, float *flow)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t nesp = esp - 6*sizeof(uint16_t); // allocated uint16_t[6] array
    uint32_t arr1Ptr = nesp;

    nesp -= 2*sizeof(uint32_t); // allocated uint32_t[2] array
    uint32_t arr2Ptr = nesp;

    m_machine->setReg(UC_X86_REG_ESP, &nesp);

    m_machine->call("GETPMP", arr1Ptr, arr2Ptr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    m_machine->read(arr1Ptr, pumps, 6*sizeof(uint16_t));
    m_machine->read(arr2Ptr, flow, 2*sizeof(float));
}

// set inlet valve
// val close (0) open (1)
// id 0-5 main recirculation pumps
void Chrnob32API::SETINL(uint32_t val, uint32_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));
    uint32_t valPtr = m_machine->push(&val, sizeof(val));

    m_machine->call("SETINL", idPtr, valPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// set discharge valve
// val close (0) open (1)
// id 0-5 main recirculation pumps
void Chrnob32API::SETDIS(uint32_t val, uint32_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));
    uint32_t valPtr = m_machine->push(&val, sizeof(val));

    m_machine->call("SETDIS", idPtr, valPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// get valve position float[12]
// index: inlet (0-5) outlet (6-11)
void Chrnob32API::GETVLV(float *res)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t nesp = esp;

    nesp -= 12*sizeof(float);
    uint32_t arrPtr = nesp;

    m_machine->setReg(UC_X86_REG_ESP, &nesp);

    m_machine->call("GETVLV", arrPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    m_machine->read(arrPtr, res, 12*sizeof(float));
}

// set reactor isolation valve
// val close (0) open (1)
// id: 0-2
// dis: inlet (0) discharge (1)
void Chrnob32API::SETISO(uint16_t val, uint16_t id, uint16_t dis)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t valPtr = m_machine->push(&val, sizeof(val));
    uint32_t idPtr = m_machine->push(&id, sizeof(id));
    uint32_t disPtr = m_machine->push(&dis, sizeof(val));

    m_machine->call("SETISO", valPtr, idPtr, disPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// get isolation valve
// id: 0-2
// dis: inlet (0) discharge (1)
float Chrnob32API::GETISO(uint16_t id, uint16_t dis)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));
    uint32_t disPtr = m_machine->push(&dis, sizeof(dis));

    m_machine->call("GETISO", idPtr, disPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint8_t st0[10];
    m_machine->reg(UC_X86_REG_ST0, &st0);

    return *reinterpret_cast<long double*>(st0);
}

// 0: drum pressure
// 1: drum temperature
// 2: drum level
float Chrnob32API::GETDRM(uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("GETDRM", idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

// returns tuple
// 1: DA level setpoint, DA manual/auto
// 3: DA level, DA manual/auto
// 4: DA temperature
// 6: DA steam valve, hotwell manual/auto
// 8: DA steam flow
// 11: DA vent valve
// 12: hotwell makeup valve
// 13: DA drain valve
// 30: hotwell setpoint
// 32: DA flow valve
std::tuple<float, uint16_t> Chrnob32API::GETDAR(uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t nesp = esp - sizeof(float);
    uint32_t retPtr = nesp;

    m_machine->setReg(UC_X86_REG_ESP, &nesp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("GETDAR", retPtr, idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    float ret1;
    m_machine->read(retPtr, &ret1, sizeof(ret1));

    uint16_t ret2;
    m_machine->reg(UC_X86_REG_AX, &ret2);

    return {ret1, ret2};
}

// 1: DA level manual (0) auto (1)
// 2: DA pressure manual (0) auto (1)
// 3: DA level setpoint
// 4: DA pressure setpoint
// 5: DA steam valve increments
// 6: DA vent valve increments
// 7: hotwell makeup valve val is increment in %
// 8: DA drain valve increments
// 13: hotwell level setpoint val is increment
// 15: DA flow valve increments
// 16: hotwell level control auto (1)/manual (0)
void Chrnob32API::SETDAR(float val, uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t valPtr = m_machine->push(&val, sizeof(val));
    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("SETDAR", valPtr, idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// set condenser
// id: 0 condenser ejector
//     1 vacuum breaker
void Chrnob32API::SETCND(float val, uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t valPtr = m_machine->push(&val, sizeof(val));
    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("SETCND", valPtr, idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// 0: condenser pressure in kPa
// 1: temperature
// 4: pump 1 temp
// 5: pump 2 temp
// 6: pump 1 on
// 7: pump 2 on
// 12, 13, 14: condensate pumps
// 17: cooling water flow
// 21: air ejector
// 22: vacuum breaker
// 28: condensate flow
// 29: steam ejector flow
// 34: hotwell level
float Chrnob32API::GETCND(uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("GETCND", idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

float Chrnob32API::GETXE(uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("GETXE", idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

// main simulation function
void Chrnob32API::REACTRX(float yitim)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t yitimPtr = m_machine->push(&yitim, sizeof(yitim));

    m_machine->call("REACTRX", yitimPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// reactor scram
// id 1: immediate scram 0 (reset) 1 (scram)
// id 2: auto scram control
void Chrnob32API::TRIPIT(uint16_t id, uint16_t val)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));
    uint32_t valPtr = m_machine->push(&val, sizeof(val));

    m_machine->call("TRIPIT", valPtr, idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// 1,2,3: condensate pumps
// 59: hotwell level auto broken
uint16_t Chrnob32API::GETMAL(uint16_t id, uint16_t mode)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));
    uint32_t modePtr = m_machine->push(&mode, sizeof(mode));

    m_machine->call("GETMAL", idPtr, modePtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint16_t ret;
    m_machine->reg(UC_X86_REG_AX, &ret);

    return ret;
}

void Chrnob32API::SETMAL(uint16_t id, uint16_t mode)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));
    uint32_t modePtr = m_machine->push(&mode, sizeof(mode));

    m_machine->call("SETMAL", idPtr, modePtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// get reactor thermal power
// out: 5*5 float array for each rod
void Chrnob32API::GETTHR(float *out)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t nesp = esp - 5*5*sizeof(float);
    uint32_t outPtr = nesp;

    m_machine->setReg(UC_X86_REG_ESP, &nesp);

    m_machine->call("GETTHR", outPtr);

    m_machine->read(outPtr, out, 5*5*sizeof(float));

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// get control rod position
// x,y 0-4
float Chrnob32API::GETROD(uint16_t x, uint16_t y)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t xPtr = m_machine->push(&x, sizeof(x));
    uint32_t yPtr = m_machine->push(&y, sizeof(y));

    m_machine->call("GETROD", xPtr, yPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

// set control rods
// x,y 0-4
// id 0 (?) 1 (insert/withdraw all) 2 (specific rod) 3 (insertion speed)
void Chrnob32API::SETROD(float val, uint32_t x, uint32_t y, uint32_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t valPtr = m_machine->push(&val, sizeof(val));
    uint32_t xPtr = m_machine->push(&x, sizeof(x));
    uint32_t yPtr = m_machine->push(&y, sizeof(y));
    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("SETROD", valPtr, xPtr, yPtr, idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// get rod movement info
// id 0 (?) 1 (rod insertion speed), 2 (center only 0 all 1)
float Chrnob32API::GETMOV(uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("GETMOV", idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

float Chrnob32API::GETMK()
{
    m_machine->call("GETMK");

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

// 3, 4: fuel temperature
// 5, 6: % voiding
float Chrnob32API::GETCOR(uint16_t num1, uint16_t num2, uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t num1Ptr = m_machine->push(&num1, sizeof(num1));
    uint32_t num2Ptr = m_machine->push(&num2, sizeof(num2));
    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("GETCOR", num1Ptr, num2Ptr, idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

// get neutron flux
float Chrnob32API::GETFLX()
{
    m_machine->call("GETFLX");

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

float Chrnob32API::GETERR()
{
    m_machine->call("GETERR");

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

float Chrnob32API::GETPRF(uint16_t id, uint16_t mode)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));
    uint32_t modePtr = m_machine->push(&mode, sizeof(mode));

    m_machine->call("SETMAL", idPtr, modePtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

// 4: ?? output 10 floats
//     [0] ecc valve flow
//     [1-2] ecc pumps flow
//     [4] ecc valve state 1 ??
//     [5-6] ecc pumps
//     [7] diesel pump
//     [8] ecc valve closed (0) open (1) auto (-1)
//     [9] ecc valve state 2 ??
void Chrnob32API::GETECC(uint16_t id, uint16_t num1, uint16_t num2, float *data, uint32_t size)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t nesp = esp - size;
    uint32_t retPtr = nesp;

    m_machine->setReg(UC_X86_REG_ESP, &nesp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));
    uint32_t num1Ptr = m_machine->push(&num1, sizeof(num1));
    uint32_t num2Ptr = m_machine->push(&num2, sizeof(num2));

    m_machine->call("GETECC", idPtr, num1Ptr, num2Ptr, retPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    m_machine->read(retPtr, data, size);
}

// -1 fuel burnup
float Chrnob32API::GETBUN(int16_t num, int16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t numPtr = m_machine->push(&num, sizeof(num));
    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("GETBUN", numPtr, idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

// offline cooling pumps
// id 1,2
// out array 3 floats
void Chrnob32API::GETSDC(int16_t id, float *out)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t nesp = esp - 3*sizeof(float);
    uint32_t outPtr = nesp;

    m_machine->setReg(UC_X86_REG_ESP, &nesp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("GETSDC", idPtr, outPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    m_machine->read(outPtr, out, 3*sizeof(float));
}

// offline cooling
// ids 1, 2
// num 1, 2
// return 2 floats
void Chrnob32API::GETSDP(int16_t id, float *out, uint16_t num)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t nesp = esp - 2*sizeof(float);
    uint32_t outPtr = nesp;

    m_machine->setReg(UC_X86_REG_ESP, &nesp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));
    uint32_t numPtr = m_machine->push(&num, sizeof(num));

    m_machine->call("GETSDP", idPtr, outPtr, numPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    m_machine->read(outPtr, out, 2*sizeof(float));
}

// 2 neutron rate
// 4 neutron flux
float Chrnob32API::GETRAT(uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("GETRAT", idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

// condensate polishers
// 1: polisher 1 remaining
// 3: polisher 3 flow
// 7: steam drum conductivity
// 15: DA conductivity
float Chrnob32API::IXCLGT(uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("IXCLGT", idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

// get turbine
// 0: control mode ???
// 9: rpm
// 15: power in MW
// 16: diff exp
// 33: hydraulic oil
// 34: lube oil
// 35: steam seal
// 36: steam drain
// 37: ?
// 38: ?
// 39: ?
float Chrnob32API::GETTRB(uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t nesp = esp - sizeof(float);
    uint32_t retPtr = nesp;

    m_machine->setReg(UC_X86_REG_ESP, &nesp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("GETTRB", retPtr, idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    float ret;
    m_machine->read(retPtr, &ret, sizeof(ret));

    return ret;
}

// set turbine
// 12: hydraulic oil
// 13: lube oil
// 14: steam seal
// 15: steam drain
void Chrnob32API::SETTRB(float val, uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t valPtr = m_machine->push(&val, sizeof(val));
    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("SETTRB", valPtr, idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// reactor automatic power regulation
// val 0 (off) 1 (on)
void Chrnob32API::SETAUT(uint16_t val)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t valPtr = m_machine->push(&val, sizeof(val));

    m_machine->call("SETAUT", valPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// 1: get reactor auto power
// 2: reactor scram
// 3: scram auto
uint32_t Chrnob32API::GETAUT(uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("GETAUT", idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint32_t ret;
    m_machine->reg(UC_X86_REG_EAX, &ret);

    return ret;
}

// set reactor power setpoint
// id: 1 (reactor setpoint increments), 0 (setpoint set)
void Chrnob32API::SETSPT(float val, uint16_t id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t valPtr = m_machine->push(&val, sizeof(val));
    uint32_t idPtr = m_machine->push(&id, sizeof(id));

    m_machine->call("SETSPT", valPtr, idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// get reactor power setpoint
float Chrnob32API::GETSPT()
{
    m_machine->call("GETSPT");

    uint8_t ret[10]; // 80bit raw float
    m_machine->reg(UC_X86_REG_ST0, &ret);
    return *reinterpret_cast<long double *>(ret); //!FIXME is this right?
}

// set reactor thermal correction
// val: 0 (off) 1 (on)
void Chrnob32API::STTCOR(uint16_t val)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t valPtr = m_machine->push(&val, sizeof(val));

    m_machine->call("STTCOR", valPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// return reactor thermal correction
// 0 (off) 1 (on)
uint16_t Chrnob32API::GTTCOR()
{
    m_machine->call("GTTCOR");

    uint32_t eax;
    m_machine->reg(UC_X86_REG_EAX, &eax);

    return eax == 1;
}

// calculates alarms
void Chrnob32API::DOALAR()
{
    m_machine->call("DOALAR");
}

QString Chrnob32API::ALARMS_GET(int id)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t nesp = esp - 0x23; // allocated 0x23 bytes for string on stack
    uint32_t strPtr = nesp;

    m_machine->setReg(UC_X86_REG_ESP, &nesp);
    uint32_t idPtr = m_machine->push(&id, sizeof(uint16_t));

    m_machine->call("ALARMS_GET", strPtr, idPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    char buff[0x23];
    m_machine->read(strPtr, buff, sizeof(buff));

    return QString::fromUtf8(buff, 0x23).trimmed();
}

// reset alarms, dont know what args means
void Chrnob32API::ALARST(int num, int stat)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t numPtr = m_machine->push(&num, sizeof(num));
    uint32_t statPtr = m_machine->push(&stat, sizeof(stat));

    m_machine->call("ALARST", numPtr, statPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}

// gets icd file size
// dont know what arg does, passing 4 seems to work and returns size/4
uint32_t Chrnob32API::GET_DATA_CNT(uint16_t in)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t inPtr = m_machine->push(&in, sizeof(in));

    m_machine->call("GET_DATA_CNT", inPtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);

    uint32_t eax;
    m_machine->reg(UC_X86_REG_EAX, &eax);

    return eax;
}

void Chrnob32API::SET_IC_DATA(char *data, uint32_t size)
{
    uint32_t esp;
    m_machine->reg(UC_X86_REG_ESP, &esp);

    uint32_t nesp = esp - size*4;
    uint32_t dataPtr = nesp;

    m_machine->write(dataPtr, data, size*4);
    m_machine->setReg(UC_X86_REG_ESP, &nesp);

    uint32_t sizePtr = m_machine->push(&size, sizeof(size));

    m_machine->call("SET_IC_DATA", dataPtr, sizePtr);

    m_machine->setReg(UC_X86_REG_ESP, &esp);
}
