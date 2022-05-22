//https://forum.arduino.cc/t/solved-problem-with-camera-module-ov7670/514763/26

// Pins configurations:
// SDA/SIOD ---> pin A4 (for Arduino UNO) | pin 20/SDA (for Arduino MEGA)
// SCL/SIOC ---> pin A5 (for Arduino UNO) | pin 21/SCL (for Arduino MEGA)
// MCLK/XCLK --> pin 11 (for Arduino UNO) | pin 10 (for Arduino MEGA)
// PCLK -------> pin 2
// VS/VSYNC ---> pin 3
// HS/HREF ----> pin 8
// D0 ---------> pin A0
// D1 ---------> pin A1
// D2 ---------> pin A2
// D3 ---------> pin A3
// D4 ---------> pin 4
// D5 ---------> pin 5
// D6 ---------> pin 6
// D7 ---------> pin 7
// RESET ------> 3.3V
// PWDN -------> GND
#include <Wire.h>
#include "registers.h"

#define baudios 1000000 // esa es la velocidad que funciona con todos los tamaños
// con 115200 solo podemos ver correctamente qqqqvga y qqqvga (ojo)

#define Width 160
#define Height 120
#define BytesPerframe 2

#define OV7670_XCLK_HZ 12000000
int miClock = 40; // 62

//#define DEBUG

// Definitions of functions for manipulating the Arduino boards pins according to each Arduino board registers, so the code will work for both Arduino UNO and Arduino MEGA:
// The only change is the connections of the SDA/SIOD, SCL/SIOC and MCLK/XCLK pins to each board (see the pins configurations above).
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // If you are using Arduino MEGA the IDE will automatically define the "__AVR_ATmega1280__" or "__AVR_ATmega2560__" constants.
#define TIMER2_PWM_A_PIN_MODE_OUTPUT() ({ DDRB |= 0b00010000; })
#define PIN2_DIGITAL_READ() ({ (PINE & 0b00010000) == 0 ? LOW : HIGH; })
#define PIN3_DIGITAL_READ() ({ (PINE & 0b00100000) == 0 ? LOW : HIGH; })
#define PIN4_DIGITAL_READ() ({ (PING & 0b00100000) == 0 ? LOW : HIGH; })
#define PIN5_DIGITAL_READ() ({ (PINE & 0b00001000) == 0 ? LOW : HIGH; })
#define PIN6_DIGITAL_READ() ({ (PINH & 0b00001000) == 0 ? LOW : HIGH; })
#define PIN7_DIGITAL_READ() ({ (PINH & 0b00010000) == 0 ? LOW : HIGH; })
#define PIN8_DIGITAL_READ() ({ (PINH & 0b00100000) == 0 ? LOW : HIGH; })
#define PINA0_DIGITAL_READ() ({ (PINF & 0b00000001) == 0 ? LOW : HIGH; })
#define PINA1_DIGITAL_READ() ({ (PINF & 0b00000010) == 0 ? LOW : HIGH; })
#define PINA2_DIGITAL_READ() ({ (PINF & 0b00000100) == 0 ? LOW : HIGH; })
#define PINA3_DIGITAL_READ() ({ (PINF & 0b00001000) == 0 ? LOW : HIGH; })
#elif defined(__AVR_ATmega328P__) // If you are using Arduino UNO the IDE will automatically define the "__AVR_ATmega328P__" constant.
#define TIMER2_PWM_A_PIN_MODE_OUTPUT() ({ DDRB |= 0b00001000; })
#define PIN2_DIGITAL_READ() ({ (PIND & 0b00000100) == 0 ? LOW : HIGH; }) // pin 2 PCLK 
#define PIN3_DIGITAL_READ() ({ (PIND & 0b00001000) == 0 ? LOW : HIGH; }) // pin 3 VSYNC
#define PIN4_DIGITAL_READ() ({ (PIND & 0b00010000) == 0 ? LOW : HIGH; })
#define PIN5_DIGITAL_READ() ({ (PIND & 0b00100000) == 0 ? LOW : HIGH; })
#define PIN6_DIGITAL_READ() ({ (PIND & 0b01000000) == 0 ? LOW : HIGH; })
#define PIN7_DIGITAL_READ() ({ (PIND & 0b10000000) == 0 ? LOW : HIGH; })
#define PIN8_DIGITAL_READ() ({ (PINB & 0b00000001) == 0 ? LOW : HIGH; }) // pin 8  HSYN
#define PINA0_DIGITAL_READ() ({ (PINC & 0b00000001) == 0 ? LOW : HIGH; })
#define PINA1_DIGITAL_READ() ({ (PINC & 0b00000010) == 0 ? LOW : HIGH; })
#define PINA2_DIGITAL_READ() ({ (PINC & 0b00000100) == 0 ? LOW : HIGH; })
#define PINA3_DIGITAL_READ() ({ (PINC & 0b00001000) == 0 ? LOW : HIGH; })
#endif

uint8_t vstart = 12;
uint8_t hstart = 210;
uint8_t edge_offset = 0;
uint8_t pclk_delay = 2;

struct OV7670_command {
  uint8_t reg;   ///< Register address
  uint8_t value; ///< Value to store
};

struct OV7670_command   NightMode[] {
  //{OV7670_REG_CLKRC, 0x80},  // no use to
  {OV7670_REG_COM11, 0xEA},
  {0xFF, 0xFF}
};

struct OV7670_command   fpsbest[] {
  {OV7670_REG_COM11, 0xEA }, //0xEA
  {OV7670_REG_DBLV, 0},
};
struct OV7670_command fps[] {
  //{OV7670_REG_CLKRC, 0x80}, // no use to
  {OV7670_REG_DBLV, 0x0A},
  {OV7670_REG_EXHCH, 0x00},
  {OV7670_REG_EXHCL, 0x00},
  {OV7670_REG_DM_LNL, 0x00},
  {0x93, 0x00},
  {OV7670_REG_COM11, 0x0A },
  {0xFF, 0xFF}
};
//320x240
struct OV7670_command  OV7670_QVGA[] {
  {OV7670_REG_COM3, 0x04},
  {OV7670_REG_COM14, 0x19 },
  {OV7670_REG_SCALING_XSC, 0x3A },
  {OV7670_REG_SCALING_YSC, 0x35 },
  {OV7670_REG_SCALING_DCWCTR, 0x11 },
  {OV7670_REG_SCALING_PCLK_DIV, 0xF1},
  {OV7670_REG_SCALING_PCLK_DELAY, 0x02 },
  {0xFF, 0xFF}
};
//160x120
struct OV7670_command  OV7670_QQVGA[] {
  {OV7670_REG_COM3, 0x04}, //0100
  {OV7670_REG_COM14, 0x1a }, // entre 4
  {OV7670_REG_SCALING_XSC, 0x3A },
  {OV7670_REG_SCALING_YSC, 0x35 },
  {OV7670_REG_SCALING_DCWCTR, 0x22 }, // ente 4
  {OV7670_REG_SCALING_PCLK_DIV, 0xF2}, //entre 4
  {OV7670_REG_SCALING_PCLK_DELAY, 0x02},
  {0xFF, 0xFF}
};
//80x60
struct OV7670_command  OV7670_QQQVGA[] {
  {OV7670_REG_COM3, 0x04},// it was 0x0C
  {OV7670_REG_COM14, 0x1b}, // entre 8
  {OV7670_REG_SCALING_XSC, 0x3A },
  {OV7670_REG_SCALING_YSC, 0x35 },
  {OV7670_REG_SCALING_DCWCTR, 0x33}, // ente 8 // la mitad se ve bien 0b00110010
  {OV7670_REG_SCALING_PCLK_DIV, 0xF3}, //entre 8
  {OV7670_REG_SCALING_PCLK_DELAY, 0x02},
  {0xFF, 0xFF}
};
//40x30
struct OV7670_command  OV7670_QQQQVGA[] {
  {OV7670_REG_COM3, 0x0C},// it was 0x0C
  {OV7670_REG_COM14, 0x1C}, // entre 8
  {OV7670_REG_SCALING_XSC, 0x40 },
  {OV7670_REG_SCALING_YSC, 0x40 },
  {OV7670_REG_SCALING_DCWCTR, 0x33 }, // ente 8 // la mitad se ve bien 0b00110010
  {OV7670_REG_SCALING_PCLK_DIV, 0xF4}, //entre 16
  {OV7670_REG_SCALING_PCLK_DELAY, 0x02},
  {0xFF, 0xFF}
};
struct OV7670_command OV7670_rgb[] =
{
  // Manual output format, RGB, use RGB565 and full 0-255 output range
  {OV7670_REG_COM7, OV7670_COM7_RGB}, // , OV7670_COM7_RGB
  {OV7670_REG_RGB444, 0},
  {OV7670_REG_COM15, OV7670_COM15_RGB565 | OV7670_COM15_R00FF},

  //{OV7670_REG_COM9, 0x38 }, /* 16x gain ceiling; 0x8 is reserved bit */

  { 0x4f, 0xb3 },   /* "matrix coefficient 1" */
  { 0x50, 0xb3 },   /* "matrix coefficient 2" */
  { 0x51, 0    },   /* vb */
  { 0x52, 0x3d },   /* "matrix coefficient 4" */
  { 0x53, 0xa7 },   /* "matrix coefficient 5" */
  { 0x54, 0xe4 },   /* "matrix coefficient 6" */

  { OV7670_REG_COM13, OV7670_COM13_GAMMA | OV7670_COM13_UVSAT },
  {0xFF, 0xFF}

};
struct OV7670_command adafruit[] = {
  {OV7670_REG_TSLB, OV7670_TSLB_YLAST},    // No auto window
  {OV7670_REG_COM10, OV7670_COM10_VS_NEG}, // -VSYNC (req by SAMD PCC)
  {OV7670_REG_SLOP, 0x20},
  {OV7670_REG_GAM_BASE, 0x1C},
  {OV7670_REG_GAM_BASE + 1, 0x28},
  {OV7670_REG_GAM_BASE + 2, 0x3C},
  {OV7670_REG_GAM_BASE + 3, 0x55},
  {OV7670_REG_GAM_BASE + 4, 0x68},
  {OV7670_REG_GAM_BASE + 5, 0x76},
  {OV7670_REG_GAM_BASE + 6, 0x80},
  {OV7670_REG_GAM_BASE + 7, 0x88},
  {OV7670_REG_GAM_BASE + 8, 0x8F},
  {OV7670_REG_GAM_BASE + 9, 0x96},
  {OV7670_REG_GAM_BASE + 10, 0xA3},
  {OV7670_REG_GAM_BASE + 11, 0xAF},
  {OV7670_REG_GAM_BASE + 12, 0xC4},
  {OV7670_REG_GAM_BASE + 13, 0xD7},
  {OV7670_REG_GAM_BASE + 14, 0xE8},
  { OV7670_REG_COM8,
    OV7670_COM8_FASTAEC | OV7670_COM8_AECSTEP | OV7670_COM8_BANDING
  },
  {OV7670_REG_GAIN, 0x00},
  {OV7670_COM2_SSLEEP, 0x00},
  {OV7670_REG_COM4, 0x00},
  {OV7670_REG_COM9, 0x20}, // Max AGC value
  {OV7670_REG_BD50MAX, 0x05},
  {OV7670_REG_BD60MAX, 0x07},
  {OV7670_REG_AEW, 0x75},
  {OV7670_REG_AEB, 0x63},
  {OV7670_REG_VPT, 0xA5},
  {OV7670_REG_HAECC1, 0x78},
  {OV7670_REG_HAECC2, 0x68},
  {0xA1, 0x03},              // Reserved register?
  {OV7670_REG_HAECC3, 0xDF}, // Histogram-based AEC/AGC setup
  {OV7670_REG_HAECC4, 0xDF},
  {OV7670_REG_HAECC5, 0xF0},
  {OV7670_REG_HAECC6, 0x90},
  {OV7670_REG_HAECC7, 0x94},
  { OV7670_REG_COM8, OV7670_COM8_FASTAEC | OV7670_COM8_AECSTEP |
    OV7670_COM8_BANDING | OV7670_COM8_AGC |
    OV7670_COM8_AEC
  },
  {OV7670_REG_COM5, 0x61},
  {OV7670_REG_COM6, 0x4B},
  {0x16, 0x02},            // Reserved register?
  {OV7670_REG_MVFP, 0x07}, // 0x07,
  {OV7670_REG_ADCCTR1, 0x02},
  {OV7670_REG_ADCCTR2, 0x91},
  {0x29, 0x07}, // Reserved register?
  {OV7670_REG_CHLF, 0x0B},
  {0x35, 0x0B}, // Reserved register?
  {OV7670_REG_ADC, 0x1D},
  {OV7670_REG_ACOM, 0x71},
  {OV7670_REG_OFON, 0x2A},
  {OV7670_REG_COM12, 0x78},
  {0x4D, 0x40}, // Reserved register?
  {0x4E, 0x20}, // Reserved register?
  {OV7670_REG_GFIX, 0x5D},
  {OV7670_REG_REG74, 0x19},
  {0x8D, 0x4F}, // Reserved register?
  {0x8E, 0x00}, // Reserved register?
  {0x8F, 0x00}, // Reserved register?
  {0x90, 0x00}, // Reserved register?
  {0x91, 0x00}, // Reserved register?
  {OV7670_REG_DM_LNL, 0x00},
  {0x96, 0x00}, // Reserved register?
  {0x9A, 0x80}, // Reserved register?
  {0xB0, 0x84}, // Reserved register?
  {OV7670_REG_ABLC1, 0x0C},
  {0xB2, 0x0E}, // Reserved register?
  {OV7670_REG_THL_ST, 0x82},
  {0xB8, 0x0A}, // Reserved register?
  {OV7670_REG_AWBC1, 0x14},
  {OV7670_REG_AWBC2, 0xF0},
  {OV7670_REG_AWBC3, 0x34},
  {OV7670_REG_AWBC4, 0x58},
  {OV7670_REG_AWBC5, 0x28},
  {OV7670_REG_AWBC6, 0x3A},
  {0x59, 0x88}, // Reserved register?
  {0x5A, 0x88}, // Reserved register?
  {0x5B, 0x44}, // Reserved register?
  {0x5C, 0x67}, // Reserved register?
  {0x5D, 0x49}, // Reserved register?
  {0x5E, 0x0E}, // Reserved register?
  {OV7670_REG_LCC3, 0x04},
  {OV7670_REG_LCC4, 0x20},
  {OV7670_REG_LCC5, 0x05},
  {OV7670_REG_LCC6, 0x04},
  {OV7670_REG_LCC7, 0x08},
  {OV7670_REG_AWBCTR3, 0x0A},
  {OV7670_REG_AWBCTR2, 0x55},
  {OV7670_REG_MTX1, 0x80},
  {OV7670_REG_MTX2, 0x80},
  {OV7670_REG_MTX3, 0x00},
  {OV7670_REG_MTX4, 0x22},
  {OV7670_REG_MTX5, 0x5E},
  {OV7670_REG_MTX6, 0x80}, // 0x40?
  {OV7670_REG_AWBCTR1, 0x11},
  {OV7670_REG_AWBCTR0, 0x9F}, // Or use 0x9E for advance AWB
  {OV7670_REG_BRIGHT, 0x00},
  {OV7670_REG_CONTRAS, 0x40},
  {OV7670_REG_CONTRAS_CENTER, 0x80}, // 0x40?
  { 0xff, 0xff }, /* END MARKER */
};
struct OV7670_command default_regs[] = {
  // { REG_COM7, COM7_RESET },
  /*
     Clock scale: 3 = 15fps
                  2 = 20fps
                  1 = 30fps
  */
  // { REG_CLKRC, 0x1 }, /* OV: clock scale (30 fps) */
  { REG_TSLB,  0x04 },  /* OV */
  // { REG_COM7, 0 },  /* VGA */
  /*
     Set the hardware window.  These values from OV don't entirely
     make sense - hstop is less than hstart.  But they work...
  */
  { REG_HSTART, 0x13 }, { REG_HSTOP, 0x01 },
  { REG_HREF, 0xb6 }, { REG_VSTART, 0x02 },
  { REG_VSTOP, 0x7a },  { REG_VREF, 0x0a },

  { REG_COM3, 0 },  { REG_COM14, 0 },
  /* Mystery scaling numbers */
  { REG_SCALING_XSC, 0x3a },
  { REG_SCALING_YSC, 0x35 },
  { 0x72, 0x11 },   { 0x73, 0xf0 },
  { 0xa2, 0x02 },   { REG_COM10, 0x0 },

  /* Gamma curve values */
  { 0x7a, 0x20 },   { 0x7b, 0x10 },
  { 0x7c, 0x1e },   { 0x7d, 0x35 },
  { 0x7e, 0x5a },   { 0x7f, 0x69 },
  { 0x80, 0x76 },   { 0x81, 0x80 },
  { 0x82, 0x88 },   { 0x83, 0x8f },
  { 0x84, 0x96 },   { 0x85, 0xa3 },
  { 0x86, 0xaf },   { 0x87, 0xc4 },
  { 0x88, 0xd7 },   { 0x89, 0xe8 },

  /* AGC and AEC parameters.  Note we start by disabling those features,
     then turn them only after tweaking the values. */
  { REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_BFILT },
  { REG_GAIN, 0 },  { REG_AECH, 0 },
  { REG_COM4, 0x40 }, /* magic reserved bit */
  { REG_COM9, 0x18 }, /* 4x gain + magic rsvd bit */
  { REG_BD50MAX, 0x05 },  { REG_BD60MAX, 0x07 },
  { REG_AEW, 0x95 },  { REG_AEB, 0x33 },
  { REG_VPT, 0xe3 },  { REG_HAECC1, 0x78 },
  { REG_HAECC2, 0x68 }, { 0xa1, 0x03 }, /* magic */
  { REG_HAECC3, 0xd8 }, { REG_HAECC4, 0xd8 },
  { REG_HAECC5, 0xf0 }, { REG_HAECC6, 0x90 },
  { REG_HAECC7, 0x94 },
  { REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_BFILT | COM8_AGC | COM8_AEC },

  /* Almost all of these are magic "reserved" values.  */
  { REG_COM5, 0x61 }, { REG_COM6, 0x4b },
  { 0x16, 0x02 },   { REG_MVFP, 0x07 },
  { 0x21, 0x02 },   { 0x22, 0x91 },
  { 0x29, 0x07 },   { 0x33, 0x0b },
  { 0x35, 0x0b },   { 0x37, 0x1d },
  { 0x38, 0x71 },   { 0x39, 0x2a },
  { REG_COM12, 0x78 },  { 0x4d, 0x40 },
  { 0x4e, 0x20 },   { REG_GFIX, 0 },
  { 0x6b, 0x4a },   { 0x74, 0x10 },
  { 0x8d, 0x4f },   { 0x8e, 0 },
  { 0x8f, 0 },    { 0x90, 0 },
  { 0x91, 0 },    { 0x96, 0 },
  { 0x9a, 0 },    { 0xb0, 0x84 },
  { 0xb1, 0x0c },   { 0xb2, 0x0e },
  { 0xb3, 0x82 },   { 0xb8, 0x0a },

  /* More reserved magic, some of which tweaks white balance */
  { 0x43, 0x0a },   { 0x44, 0xf0 },
  { 0x45, 0x34 },   { 0x46, 0x58 },
  { 0x47, 0x28 },   { 0x48, 0x3a },
  { 0x59, 0x88 },   { 0x5a, 0x88 },
  { 0x5b, 0x44 },   { 0x5c, 0x67 },
  { 0x5d, 0x49 },   { 0x5e, 0x0e },
  { 0x6c, 0x0a },   { 0x6d, 0x55 },
  { 0x6e, 0x11 },   { 0x6f, 0x9f }, /* "9e for advance AWB" */
  { 0x6a, 0x40 },   { REG_BLUE, 0x40 },
  { REG_RED, 0x60 },
  { REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_BFILT | COM8_AGC | COM8_AEC | COM8_AWB },

  /* Matrix coefficients */
  { 0x4f, 0x80 },   { 0x50, 0x80 },
  { 0x51, 0 },    { 0x52, 0x22 },
  { 0x53, 0x5e },   { 0x54, 0x80 },
  { 0x58, 0x9e }, // 0b00011010

  { REG_COM16, COM16_AWBGAIN }, { REG_EDGE, 0 },
  { 0x75, 0x05 },   { 0x76, 0xe1 },
  { 0x4c, 0 },    { 0x77, 0x01 },
  { REG_COM13, 0xc3 },  { 0x4b, 0x09 },
  { 0xc9, 0x60 },   { REG_COM16, 0x38 },
  { 0x56, 0x40 },

  { 0x34, 0x11 },   { REG_COM11, COM11_EXP | COM11_HZAUTO },
  { 0xa4, 0x88 },   { 0x96, 0 },
  { 0x97, 0x30 },   { 0x98, 0x20 },
  { 0x99, 0x30 },   { 0x9a, 0x84 },
  { 0x9b, 0x29 },   { 0x9c, 0x03 },
  { 0x9d, 0x4c },   { 0x9e, 0x3f },
  { 0x78, 0x04 },

  /* Extra-weird stuff.  Some sort of multiplexor register */
  { 0x79, 0x01 },   { 0xc8, 0xf0 },
  { 0x79, 0x0f },   { 0xc8, 0x00 },
  { 0x79, 0x10 },   { 0xc8, 0x7e },
  { 0x79, 0x0a },   { 0xc8, 0x80 },
  { 0x79, 0x0b },   { 0xc8, 0x01 },
  { 0x79, 0x0c },   { 0xc8, 0x0f },
  { 0x79, 0x0d },   { 0xc8, 0x20 },
  { 0x79, 0x09 },   { 0xc8, 0x80 },
  { 0x79, 0x02 },   { 0xc8, 0xc0 },
  { 0x79, 0x03 },   { 0xc8, 0x40 },
  { 0x79, 0x05 },   { 0xc8, 0x30 },
  { 0x79, 0x26 },
  // esto es para manipular el brillo y contraste
  //{OV7670_REG_AWBCTR0, 0x9F}, // Or use 0x9E for advance AWB
  //{OV7670_REG_BRIGHT, 0b10000000 | 0x30},
  //{OV7670_REG_BRIGHT, 0x00},
  //{OV7670_REG_CONTRAS, 0x40},
  //{OV7670_REG_CONTRAS_CENTER, 0x10}, // 0x40?
  //{OV7670_REG_LAST + 1, 0x00},       // End-of-data marker

  { 0xff, 0xff }, /* END MARKER */
};
void SetupForframe() {
  uint16_t vstop = vstart + 480;
  uint16_t hstop = (hstart + 640) % 784;
  Write(OV7670_REG_HSTART, hstart >> 3);
  Write(OV7670_REG_HSTOP, hstop >> 3);
  Write(OV7670_REG_HREF, (edge_offset << 6) | ((hstop & 0b111) << 3) | (hstart & 0b111));
  Write(OV7670_REG_VSTART, vstart >> 2);
  Write(OV7670_REG_VSTOP, vstop >> 2);
  Write(OV7670_REG_VREF, ((vstop & 0b11) << 2) | (vstart & 0b11));
}

void initializePWMTimer() {
  cli();
  TIMER2_PWM_A_PIN_MODE_OUTPUT(); // Set the A PWM pin of TIMER2 to output
  ASSR &= ~(_BV(EXCLK) | _BV(AS2));
  TCCR2A = (1 << COM2A0) | (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << WGM22) | (1 << CS20);
  OCR2A = 0;
  sei();
}

void mycaptureFrame(unsigned int frameWidth, unsigned int frameHeight) {
#ifdef DEBUG
  uint16_t contador = 0;
#endif

  cli();// disable all interrupts during frame capture (because it needs to be as fast as possible
  //uint8_t* b = (uint8_t*)buffer;
  int bytesPerPixel = 2;
  uint16_t bytesPerRow = frameWidth * bytesPerPixel;
  // Falling edge indicates start of frame  vsyncPort

  while (PIN3_DIGITAL_READ() == LOW); // wait until VS/VSYNC pin is high
  while (PIN3_DIGITAL_READ() == HIGH); // wait until VS/VSYNC pin is low
  for (uint16_t i = 0; i < frameHeight; i++) {
    // rising edge indicates start of line
    while (PIN8_DIGITAL_READ() == LOW); // wait for HIGH
    for (uint16_t j = 0; j < bytesPerRow; j++) {

      // rising edges clock each data byte
      while (PIN2_DIGITAL_READ() == HIGH); // wait until PCLK pin is low

      byte byteToWrite = 0b00000000;

      byteToWrite |= ((PIN7_DIGITAL_READ() == HIGH) << 7);
      byteToWrite |= ((PIN6_DIGITAL_READ() == HIGH) << 6);
      byteToWrite |= ((PIN5_DIGITAL_READ() == HIGH) << 5);
      byteToWrite |= ((PIN4_DIGITAL_READ() == HIGH) << 4);
      byteToWrite |= ((PINA3_DIGITAL_READ() == HIGH) << 3);
      byteToWrite |= ((PINA2_DIGITAL_READ() == HIGH) << 2);
      byteToWrite |= ((PINA1_DIGITAL_READ() == HIGH) << 1);
      byteToWrite |= ((PINA0_DIGITAL_READ() == HIGH));

      //UDR0 = byteToWrite;
#ifdef DEBUG
      contador++;
#else
      Serial.write(byteToWrite);
#endif
      while (PIN2_DIGITAL_READ() == LOW); // wait until PCLK pin is high
    }
    while (PIN8_DIGITAL_READ() == HIGH); // wait for LOW
  }

  sei(); // enable all interrupts
#ifdef DEBUG
  Serial.print("\n Cantidad de Bytes esperados : ");
  uint16_t total = Width * Height * BytesPerframe;
  Serial.println(total);
  Serial.print(" Cantidad de Bytes escritos  : ");
  Serial.println(contador);
#endif
}

void Write(int RegisterAddress, int value) {
  Wire.beginTransmission(SCCB_ADDRESS);
  Wire.write(RegisterAddress);
  Wire.write(value);
  Wire.endTransmission();
  delay(1);
}

void write_list(struct OV7670_command *cmd) {
  for (int i = 0; cmd[i].reg <= OV7670_REG_LAST; i++) {
    Write(cmd[i].reg, cmd[i].value);
    delay(1); // Required, else lockup on init
  }
}



float OV7670_set_fps(float fps) {

  // Pixel clock (PCLK), which determines overall frame rate, is a
  // function of XCLK input frequency (OV7670_XCLK_HZ), a PLL multiplier
  // and then an integer division factor (1-32). These are the available
  // OV7670 PLL ratios:
  static const uint8_t pll_ratio[] = {1, 4, 6, 8};
  const uint8_t num_plls = sizeof pll_ratio / sizeof pll_ratio[0];

  // Constrain frame rate to upper and lower limits
  fps = (fps > 30) ? 30 : fps;               // Max 30 FPS
  float pclk_target = fps * 4000000.0 / 5.0; // Ideal PCLK Hz for target FPS
  uint32_t pclk_min = OV7670_XCLK_HZ / 32;   // Min PCLK determines min FPS
  if (pclk_target < (float)pclk_min) {       // If PCLK target is below limit
    if (true) {
      writeCameraRegister(OV7670_REG_DBLV, 0);   // 1:1 PLL
      writeCameraRegister(OV7670_REG_CLKRC, 31); // 1/32 div
    }
    return (float)(pclk_min * 5 / 4000000); // Return min frame rate
  }

  // Find nearest available FPS without going over. This is done in a
  // brute-force manner, testing all 127 PLL-up by divide-down permutations
  // and tracking the best fit. I’m certain there are shortcuts but was
  // having trouble with my math, might revisit later. It's not a huge
  // bottleneck...MCUs are fast now, many cases are quickly discarded, and
  // this operation is usually done only once on startup (the I2C transfers
  // probably take longer).

  uint8_t best_pll = 0;    // Index (not value) of best PLL match
  uint8_t best_div = 1;    // Value of best division factor match
  float best_delta = 30.0; // Best requested vs actual FPS (init to "way off")

  for (uint8_t p = 0; p < num_plls; p++) {
    uint32_t xclk_pll = OV7670_XCLK_HZ * pll_ratio[p]; // PLL'd freq
    uint8_t first_div = p ? 2 : 1; // Min div is 1 for PLL 1:1, else 2
    for (uint8_t div = first_div; div <= 32; div++) {
      uint32_t pclk_result = xclk_pll / div; // PCLK-up-down permutation
      if (pclk_result > pclk_target) {       // Exceeds target?
        continue;                            //  Skip it
      }
      float fps_result = (float)pclk_result * 5.0 / 4000000.0; // 4000000.0
      float delta = fps - fps_result; // How far off?
      if (delta < best_delta) {       // Best match yet?
        best_delta = delta;           //  Save delta,
        best_pll = p;                 //  pll and
        best_div = div;               //  div for later use
      }
    }
  }

  if (true) {
    // Set up DBLV and CLKRC registers with best PLL and div values
    if (pll_ratio[best_pll] == best_div) { // If PLL and div are same (1:1)
      // Bypass PLL, use external clock directly
      writeCameraRegister(OV7670_REG_DBLV, 0);
      writeCameraRegister(OV7670_REG_CLKRC, 0x40);
    } else {
      // Set DBLV[7:6] for PLL, CLKRC[5:0] for div-1 (1-32 stored as 0-31)
      writeCameraRegister(OV7670_REG_DBLV, best_pll << 6);
      writeCameraRegister(OV7670_REG_CLKRC, best_div - 1);
    }
  }


  //return best_pll;
#ifdef DEBUG
  Serial.print("best fps : ");
  Serial.println(fps - best_delta);
  Serial.print("best pll : ");
  Serial.println(best_pll);
#endif
  return fps - best_delta; // Return actual frame rate
}

// valores iniciale para los Frames
void SetupCamera() {
  writeCameraRegister(0x12, 0x80); // reset all camera registers to default value
  delay(1000);
  write_list(default_regs);// adafruit(dom't work),  default_regs
  write_list(fpsbest); // it's working
  write_list(OV7670_rgb);
  if (Height == 30) {
    //qqqqvga
    write_list(OV7670_QQQQVGA);
    vstart = 15;
    hstart = 252;
    edge_offset = 3;
  } else if (Height == 60) {
    //qqqvga
    write_list(OV7670_QQQVGA);
    vstart = 12;
    hstart = 210;
    edge_offset = 0;
  } else if (Height == 120) {
    //qqvga
    write_list(OV7670_QQVGA);
    vstart = 11;
    hstart = 186;
    edge_offset = 2;
  } else {
    //qvga No por ahora
    write_list(OV7670_QVGA);
    vstart = 10;
    hstart = 174;
    edge_offset = 4;
  }
  SetupForframe();
  
  //OV7670_set_fps(18);
  delay(100);
  writeCameraRegister(0x11, miClock); // 62 works for qqqvga
}

void setup() {
  initializePWMTimer();
  Wire.begin();
  Serial.begin(baudios);
  SetupCamera();
  /*
    read_reg(OV7670_REG_COM7);
    read_reg(OV7670_REG_RGB444);
    read_reg(OV7670_REG_COM15);
    read_reg(0x4f);
    read_reg(0x50);
    read_reg(0x51);
    read_reg(0x52);
    read_reg(0x53);
    read_reg(0x54);
    read_reg(OV7670_REG_COM13);
    read_reg(0x11);
  */

  mycaptureFrame(Width, Height); // 88 x 72

}



byte read_reg(byte regNumber ) {
  byte val;
  // LETTURA DEL REGISTRO
  Wire.beginTransmission(CAMERA_ADDRESS);
  Wire.write(regNumber);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)CAMERA_ADDRESS, (uint8_t)1);
  val = Wire.read();
#ifdef DEBUG
  //DEBUG:
  Serial.print("[0x");
  Serial.print(regNumber, HEX);
  Serial.print("] ");
  Serial.print(" Value = 0x");
  Serial.println(val, HEX);
#endif
  return val;
}

void loop() {
  // do nothig for now
}
