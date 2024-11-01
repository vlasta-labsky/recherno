#ifndef API_H
#define API_H

#include <cstdint>

#include <QString>

class VirtualMachine;

class Chrnob32API
{
public:
    Chrnob32API(VirtualMachine *machine)
        : m_machine(machine){}

    void SETFED(float val, uint16_t id);
    float GETFED(uint16_t id);

    void SETPMP(uint16_t id, int16_t val);
    void GETPMP(uint16_t *pumps, float *flow);

    void SETINL(uint32_t val, uint32_t id);
    void SETDIS(uint32_t val, uint32_t id);
    void GETVLV(float *res);

    void SETISO(uint16_t val, uint16_t id, uint16_t dis);
    float GETISO(uint16_t id, uint16_t dis);

    float GETDRM(uint16_t id);

    std::tuple<float, uint16_t> GETDAR(uint16_t id);
    void SETDAR(float val, uint16_t id);

    void SETCND(float val, uint16_t id);
    float GETCND(uint16_t id);

    float GETTRB(uint16_t id);
    void SETTRB(float val, uint16_t id);

    void SETAUT(uint16_t val);
    uint32_t GETAUT(uint16_t id);

    void SETSPT(float val, uint16_t id);
    float GETSPT();

    void STTCOR(uint16_t val);
    uint16_t GTTCOR();

    float GETXE(uint16_t id);
    void REACTRX(float yitim);

    void TRIPIT(uint16_t id, uint16_t val);

    uint16_t GETMAL(uint16_t id, uint16_t mode);
    void SETMAL(uint16_t id, uint16_t mode);

    void GETTHR(float *out);

    float GETROD(uint16_t x, uint16_t y);
    void SETROD(float val, uint32_t x, uint32_t y, uint32_t id);

    float GETMOV(uint16_t id);

    float GETMK();

    float GETCOR(uint16_t num1, uint16_t num2, uint16_t id);

    float GETFLX();
    float GETERR();

    float GETPRF(uint16_t id, uint16_t mode);

    void GETECC(uint16_t id, uint16_t num1, uint16_t num2, float *data, uint32_t size);

    float GETBUN(int16_t num, int16_t id);

    void GETSDC(int16_t id, float *out);
    void GETSDP(int16_t id, float *out, uint16_t num);

    float GETRAT(uint16_t id);

    float IXCLGT(uint16_t id);

    void DOALAR();
    QString ALARMS_GET(int id);
    void ALARST(int num, int stat);

    uint32_t GET_DATA_CNT(uint16_t in);
    void SET_IC_DATA(char *data, uint32_t size);

protected:
    VirtualMachine *m_machine;
};

#endif // API_H
