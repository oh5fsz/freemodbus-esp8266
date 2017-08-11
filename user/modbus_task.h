#ifndef MODBUS_TASK_H
#define MODBUS_TASK_H

#define MODBUS_ADDRESS                  0x0A
#define MODBUS_BAUDRATE                 19200

typedef uint16_t USHORT;
#define REG_INPUT_START                 ( 1000 )
#define REG_INPUT_NREGS                 ( 64 )

#define REG_HOLDING_START               ( 1 )
#define REG_HOLDING_NREGS               ( 16 )

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];
static USHORT   usRegHoldingStart = REG_HOLDING_START;
static USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

void ModbusTask(void *pvParams);
#endif // MODBUS_TASK_H
