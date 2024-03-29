#define ROAD 64
#define TOWN 65
#define STOP 66
#define CAST 67

const char overworld[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //0
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //1
    0x00, ROAD, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, ROAD, ROAD,
    ROAD, ROAD, ROAD, ROAD, ROAD,
    ROAD, ROAD, ROAD, ROAD, ROAD, //2
    ROAD, 0x00, ROAD, ROAD, ROAD,
    ROAD, 0x04, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, ROAD,
    0x00, ROAD, ROAD, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //3
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, ROAD, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, ROAD, ROAD, 0x00,
    ROAD, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //4
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, ROAD, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, ROAD, 0x00, 0x00, 0x00,
    0x00, ROAD, ROAD, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //5
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    ROAD, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, ROAD, ROAD,
    ROAD, 0x00, 0x00, 0x00, 0x00, //6
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    ROAD, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,

    ROAD, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, ROAD, 0x00, 0x00, 0x00, //7
    0x00, 0x00, ROAD, ROAD, ROAD,
    ROAD, ROAD, 0x00, 0x00, 0x00,
    ROAD, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, ROAD, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, ROAD, 0x00, 0x00,
    0x00, ROAD, 0x00, 0x00, 0x00, //8
    0x00, 0x00, ROAD, ROAD, 0x88,
    ROAD, ROAD, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, ROAD, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, ROAD, 0x00,
    0x00, ROAD, 0x00, 0x00, 0x00, //9
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, ROAD, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, ROAD, 0x00, ROAD, ROAD,
    ROAD, ROAD, 0x00, 0x00, ROAD,
    ROAD, ROAD, ROAD, 0x07, ROAD, //10
    ROAD, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, ROAD, 0x00, 0x00,
    0x81, 0x00,

    0x00, ROAD, 0x00, 0x00, 0x00,
    0x00, 0x00, ROAD, ROAD, 0x00,
    0x00, 0x00, 0x00, ROAD, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //11
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, ROAD, 0x00, ROAD, ROAD,
    0x00, 0x00,

    0x03, ROAD, ROAD, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, ROAD, 0x00, ROAD,
    0x00, 0x00, 0x00, 0x00, 0x00, //12
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, ROAD, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, 0x00, ROAD, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, ROAD, 0x00, ROAD,
    ROAD, ROAD, 0x00, 0x00, 0x00, //13
    0x00, 0x00, 0x00, 0x80, ROAD,
    ROAD, 0x00, ROAD, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, ROAD, 0x00, ROAD,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, ROAD, 0x00, 0x00,
    0x00, 0x00, ROAD, 0x00, 0x00, //14
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, ROAD, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, ROAD, 0x00, 0x00,
    ROAD, 0x00, 0x00, 0x00, 0x00,
    0x00, ROAD, 0x00, 0x00, 0x00, //15
    0x00, 0x00, ROAD, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, ROAD, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, ROAD, 0x00, 0x00,
    0x00, ROAD, 0x00, 0x00, 0x00,
    ROAD, 0x00, 0x00, 0x00, ROAD,
    ROAD, ROAD, 0x00, ROAD, 0x00, //16
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0X20, 0x00,
    0x00, 0x00,

    0x00, 0x00, ROAD, 0x00, 0x00,
    0x00, 0x00, ROAD, ROAD, ROAD,
    0x09, ROAD, ROAD, ROAD, 0x00, //17
    0x00, 0x00, 0x00, 0x00, ROAD,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, STOP, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //18
    0x00, 0x00, ROAD, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00,
    CAST, ROAD, ROAD, ROAD, STOP,
    ROAD, ROAD, ROAD, 0x00, 0x00, //19
    0x00, ROAD, STOP, 0x00, 0x00,
    0x00, 0x00, ROAD, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, ROAD,
    0x00, 0x00, 0x00, 0x00, ROAD,
    0x00, 0x00, 0x00, ROAD, STOP,
    ROAD, 0x00, 0x00, 0x00, 0x00, //20
    0x00, 0x00, ROAD, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, ROAD, ROAD, 0x00,
    0x00, 0x00, 0x00, 0x00, ROAD,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //21
    0x00, 0x00, ROAD, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, ROAD, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, TOWN,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //22
    0x00, 0x00, 0x00, STOP, ROAD,
    ROAD, STOP, ROAD, 0x00, 0x00,
    0x00, 0x00,

    0x00, STOP, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    ROAD, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //23
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, ROAD, 0x00,
    0x00, 0x00,

    0x00, ROAD, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, ROAD, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //24
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, CAST, 0x00,
    0x00, 0x00,

    0x00, 0x00, ROAD, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, CAST, 0x00, 0x00, 0x00, //25
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, ROAD, 0x00,
    0x00, 0x00,

    0x00, 0x00, 0x00, ROAD, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, ROAD, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //26
    TOWN, ROAD, ROAD, 0x00, 0x00,
    0x00, 0x00, 0x00, ROAD, 0x00,
    0x00, 0x00,

    0x00, 0x00, 0x00, TOWN, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    ROAD, 0x00, 0x00, 0x00, 0x00, //27
    0x00, 0x00, 0x00, 0x00, ROAD,
    0x00, 0x00, 0x00, ROAD, TOWN,
    ROAD, ROAD, TOWN, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, 0x00, ROAD, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    ROAD, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, ROAD, 0x00, //28
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, 0x00, ROAD, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    ROAD, 0x00, 0x00, ROAD, ROAD,
    ROAD, ROAD, ROAD, TOWN, 0x00, //29
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, ROAD,
    0x00, 0x00, 0x00, 0x00, 0x00,
    TOWN, ROAD, ROAD, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //30
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00,
    CAST, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, //31
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00
};
// const int one_to_two = {0x6, 0x7, 0x18, 0x29, 0x1A, 0x1B, 0x1C, 0xC};
