#include <Wire.h>

// aquí estan os registro de adafruit

#define OV7670_ADDR 0x21 //< Default I2C address if unspecified

#define CAMERA_ADDRESS 0x21
#define SCCB_ADDRESS  0x21

#define OV7670_REG_GAIN 0x00               //< AGC gain bits 7:0 (9:8 in VREF)
#define OV7670_REG_BLUE 0x01               //< AWB blue channel gain
#define OV7670_REG_RED 0x02                //< AWB red channel gain
#define OV7670_REG_VREF 0x03               //< Vert frame control bits
#define OV7670_REG_COM1 0x04               //< Common control 1
#define OV7670_COM1_R656 0x40              //< COM1 enable R656 format
#define OV7670_REG_BAVE 0x05               //< U/B average level
#define OV7670_REG_GbAVE 0x06              //< Y/Gb average level
#define OV7670_REG_AECHH 0x07              //< Exposure value - AEC 15:10 bits
#define OV7670_REG_RAVE 0x08               //< V/R average level
#define OV7670_REG_COM2 0x09               //< Common control 2
#define OV7670_COM2_SSLEEP 0x10            //< COM2 soft sleep mode
#define OV7670_REG_PID 0x0A                //< Product ID MSB (read-only)
#define OV7670_REG_VER 0x0B                //< Product ID LSB (read-only)
#define OV7670_REG_COM3 0x0C               //< Common control 3
#define OV7670_COM3_SWAP 0x40              //< COM3 output data MSB/LSB swap
#define OV7670_COM3_SCALEEN 0x08           //< COM3 scale enable
#define OV7670_COM3_DCWEN 0x04             //< COM3 DCW enable
#define OV7670_REG_COM4 0x0D               //< Common control 4
#define OV7670_REG_COM5 0x0E               //< Common control 5
#define OV7670_REG_COM6 0x0F               //< Common control 6
#define OV7670_REG_AECH 0x10               //< Exposure value 9:2
#define OV7670_REG_CLKRC 0x11              //< Internal clock
#define OV7670_CLK_EXT 0x40                //< CLKRC Use ext clock directly
#define OV7670_CLK_SCALE 0x3F              //< CLKRC Int clock prescale mask
#define OV7670_REG_COM7 0x12               //< Common control 7
#define OV7670_COM7_RESET 0x80             //< COM7 SCCB register reset
#define OV7670_COM7_SIZE_MASK 0x38         //< COM7 output size mask
#define OV7670_COM7_PIXEL_MASK 0x05        //< COM7 output pixel format mask
#define OV7670_COM7_SIZE_VGA 0x00          //< COM7 output size VGA
#define OV7670_COM7_SIZE_CIF 0x20          //< COM7 output size CIF
#define OV7670_COM7_SIZE_QVGA 0x10         //< COM7 output size QVGA
#define OV7670_COM7_SIZE_QCIF 0x08         //< COM7 output size QCIF
#define OV7670_COM7_RGB 0x04               //< COM7 pixel format RGB
#define OV7670_COM7_YUV 0x00               //< COM7 pixel format YUV
#define OV7670_COM7_BAYER 0x01             //< COM7 pixel format Bayer RAW
#define OV7670_COM7_PBAYER 0x05            //< COM7 pixel fmt proc Bayer RAW
#define OV7670_COM7_COLORBAR 0x02          //< COM7 color bar enable
#define OV7670_REG_COM8 0x13               //< Common control 8
#define OV7670_COM8_FASTAEC 0x80           //< COM8 Enable fast AGC/AEC algo,
#define OV7670_COM8_AECSTEP 0x40           //< COM8 AEC step size unlimited
#define OV7670_COM8_BANDING 0x20           //< COM8 Banding filter enable
#define OV7670_COM8_AGC 0x04               //< COM8 AGC (auto gain) enable
#define OV7670_COM8_AWB 0x02               //< COM8 AWB (auto white balance)
#define OV7670_COM8_AEC 0x01               //< COM8 AEC (auto exposure) enable
#define OV7670_REG_COM9 0x14               //< Common control 9 - max AGC value
#define OV7670_REG_COM10 0x15              //< Common control 10
#define OV7670_COM10_HSYNC 0x40            //< COM10 HREF changes to HSYNC
#define OV7670_COM10_PCLK_HB 0x20          //< COM10 Suppress PCLK on hblank
#define OV7670_COM10_HREF_REV 0x08         //< COM10 HREF reverse
#define OV7670_COM10_VS_EDGE 0x04          //< COM10 VSYNC chg on PCLK rising
#define OV7670_COM10_VS_NEG 0x02           //< COM10 VSYNC negative
#define OV7670_COM10_HS_NEG 0x01           //< COM10 HSYNC negative
#define OV7670_REG_HSTART 0x17             //< Horiz frame start high bits
#define OV7670_REG_HSTOP 0x18              //< Horiz frame end high bits
#define OV7670_REG_VSTART 0x19             //< Vert frame start high bits
#define OV7670_REG_VSTOP 0x1A              //< Vert frame end high bits
#define OV7670_REG_PSHFT 0x1B              //< Pixel delay select
#define OV7670_REG_MIDH 0x1C               //< Manufacturer ID high byte
#define OV7670_REG_MIDL 0x1D               //< Manufacturer ID low byte
#define OV7670_REG_MVFP 0x1E               //< Mirror / vert-flip enable
#define OV7670_MVFP_MIRROR 0x20            //< MVFP Mirror image
#define OV7670_MVFP_VFLIP 0x10             //< MVFP Vertical flip
#define OV7670_REG_LAEC 0x1F               //< Reserved
#define OV7670_REG_ADCCTR0 0x20            //< ADC control
#define OV7670_REG_ADCCTR1 0x21            //< Reserved
#define OV7670_REG_ADCCTR2 0x22            //< Reserved
#define OV7670_REG_ADCCTR3 0x23            //< Reserved
#define OV7670_REG_AEW 0x24                //< AGC/AEC upper limit
#define OV7670_REG_AEB 0x25                //< AGC/AEC lower limit
#define OV7670_REG_VPT 0x26                //< AGC/AEC fast mode op region
#define OV7670_REG_BBIAS 0x27              //< B channel signal output bias
#define OV7670_REG_GbBIAS 0x28             //< Gb channel signal output bias
#define OV7670_REG_EXHCH 0x2A              //< Dummy pixel insert MSB
#define OV7670_REG_EXHCL 0x2B              //< Dummy pixel insert LSB
#define OV7670_REG_RBIAS 0x2C              //< R channel signal output bias
#define OV7670_REG_ADVFL 0x2D              //< Insert dummy lines MSB
#define OV7670_REG_ADVFH 0x2E              //< Insert dummy lines LSB
#define OV7670_REG_YAVE 0x2F               //< Y/G channel average value
#define OV7670_REG_HSYST 0x30              //< HSYNC rising edge delay
#define OV7670_REG_HSYEN 0x31              //< HSYNC falling edge delay
#define OV7670_REG_HREF 0x32               //< HREF control
#define OV7670_REG_CHLF 0x33               //< Array current control
#define OV7670_REG_ARBLM 0x34              //< Array ref control - reserved
#define OV7670_REG_ADC 0x37                //< ADC control - reserved
#define OV7670_REG_ACOM 0x38               //< ADC & analog common - reserved
#define OV7670_REG_OFON 0x39               //< ADC offset control - reserved
#define OV7670_REG_TSLB 0x3A               //< Line buffer test option
#define OV7670_TSLB_NEG 0x20               //< TSLB Negative image enable
#define OV7670_TSLB_YLAST 0x04             //< TSLB UYVY or VYUY, see COM13
#define OV7670_TSLB_AOW 0x01               //< TSLB Auto output window
#define OV7670_REG_COM11 0x3B              //< Common control 11
#define OV7670_COM11_NIGHT 0x80            //< COM11 Night mode
#define OV7670_COM11_NMFR 0x60             //< COM11 Night mode frame rate mask
#define OV7670_COM11_HZAUTO 0x10           //< COM11 Auto detect 50/60 Hz
#define OV7670_COM11_BAND 0x08             //< COM11 Banding filter val select
#define OV7670_COM11_EXP 0x02              //< COM11 Exposure timing control
#define OV7670_REG_COM12 0x3C              //< Common control 12
#define OV7670_COM12_HREF 0x80             //< COM12 Always has HREF
#define OV7670_REG_COM13 0x3D              //< Common control 13
#define OV7670_COM13_GAMMA 0x80            //< COM13 Gamma enable
#define OV7670_COM13_UVSAT 0x40            //< COM13 UV saturation auto adj
#define OV7670_COM13_UVSWAP 0x01           //< COM13 UV swap, use w TSLB[3]
#define OV7670_REG_COM14 0x3E              //< Common control 14
#define OV7670_COM14_DCWEN 0x10            //< COM14 DCW & scaling PCLK enable
#define OV7670_REG_EDGE 0x3F               //< Edge enhancement adjustment
#define OV7670_REG_COM15 0x40              //< Common control 15
#define OV7670_COM15_RMASK 0xC0            //< COM15 Output range mask
#define OV7670_COM15_R10F0 0x00            //< COM15 Output range 10 to F0
#define OV7670_COM15_R01FE 0x80            //< COM15              01 to FE
#define OV7670_COM15_R00FF 0xC0            //< COM15              00 to FF
#define OV7670_COM15_RGBMASK 0x30          //< COM15 RGB 555/565 option mask
#define OV7670_COM15_RGB 0x00              //< COM15 Normal RGB out
#define OV7670_COM15_RGB565 0x10           //< COM15 RGB 565 output
#define OV7670_COM15_RGB555 0x30           //< COM15 RGB 555 output
#define OV7670_REG_COM16 0x41              //< Common control 16
#define OV7670_COM16_AWBGAIN 0x08          //< COM16 AWB gain enable
#define OV7670_REG_COM17 0x42              //< Common control 17
#define OV7670_COM17_AECWIN 0xC0           //< COM17 AEC window must match COM4
#define OV7670_COM17_CBAR 0x08             //< COM17 DSP Color bar enable
#define OV7670_REG_AWBC1 0x43              //< Reserved
#define OV7670_REG_AWBC2 0x44              //< Reserved
#define OV7670_REG_AWBC3 0x45              //< Reserved
#define OV7670_REG_AWBC4 0x46              //< Reserved
#define OV7670_REG_AWBC5 0x47              //< Reserved
#define OV7670_REG_AWBC6 0x48              //< Reserved
#define OV7670_REG_REG4B 0x4B              //< UV average enable
#define OV7670_REG_DNSTH 0x4C              //< De-noise strength
#define OV7670_REG_MTX1 0x4F               //< Matrix coefficient 1
#define OV7670_REG_MTX2 0x50               //< Matrix coefficient 2
#define OV7670_REG_MTX3 0x51               //< Matrix coefficient 3
#define OV7670_REG_MTX4 0x52               //< Matrix coefficient 4
#define OV7670_REG_MTX5 0x53               //< Matrix coefficient 5
#define OV7670_REG_MTX6 0x54               //< Matrix coefficient 6
#define OV7670_REG_BRIGHT 0x55             //< Brightness control
#define OV7670_REG_CONTRAS 0x56            //< Contrast control
#define OV7670_REG_CONTRAS_CENTER 0x57     //< Contrast center
#define OV7670_REG_MTXS 0x58               //< Matrix coefficient sign
#define OV7670_REG_LCC1 0x62               //< Lens correction option 1
#define OV7670_REG_LCC2 0x63               //< Lens correction option 2
#define OV7670_REG_LCC3 0x64               //< Lens correction option 3
#define OV7670_REG_LCC4 0x65               //< Lens correction option 4
#define OV7670_REG_LCC5 0x66               //< Lens correction option 5
#define OV7670_REG_MANU 0x67               //< Manual U value
#define OV7670_REG_MANV 0x68               //< Manual V value
#define OV7670_REG_GFIX 0x69               //< Fix gain control
#define OV7670_REG_GGAIN 0x6A              //< G channel AWB gain
#define OV7670_REG_DBLV 0x6B               //< PLL & regulator control
#define OV7670_REG_AWBCTR3 0x6C            //< AWB control 3
#define OV7670_REG_AWBCTR2 0x6D            //< AWB control 2
#define OV7670_REG_AWBCTR1 0x6E            //< AWB control 1
#define OV7670_REG_AWBCTR0 0x6F            //< AWB control 0
#define OV7670_REG_SCALING_XSC 0x70        //< Test pattern X scaling
#define OV7670_REG_SCALING_YSC 0x71        //< Test pattern Y scaling
#define OV7670_REG_SCALING_DCWCTR 0x72     //< DCW control
#define OV7670_REG_SCALING_PCLK_DIV 0x73   //< DSP scale control clock divide
#define OV7670_REG_REG74 0x74              //< Digital gain control
#define OV7670_REG_REG76 0x76              //< Pixel correction
#define OV7670_REG_SLOP 0x7A               //< Gamma curve highest seg slope
#define OV7670_REG_GAM_BASE 0x7B           //< Gamma register base (1 of 15)
#define OV7670_GAM_LEN 15                  //< Number of gamma registers
#define OV7670_R76_BLKPCOR 0x80            //< REG76 black pixel corr enable
#define OV7670_R76_WHTPCOR 0x40            //< REG76 white pixel corr enable
#define OV7670_REG_RGB444 0x8C             //< RGB 444 control
#define OV7670_R444_ENABLE 0x02            //< RGB444 enable
#define OV7670_R444_RGBX 0x01              //< RGB444 word format
#define OV7670_REG_DM_LNL 0x92             //< Dummy line LSB
#define OV7670_REG_LCC6 0x94               //< Lens correction option 6
#define OV7670_REG_LCC7 0x95               //< Lens correction option 7
#define OV7670_REG_HAECC1 0x9F             //< Histogram-based AEC/AGC ctrl 1
#define OV7670_REG_HAECC2 0xA0             //< Histogram-based AEC/AGC ctrl 2
#define OV7670_REG_SCALING_PCLK_DELAY 0xA2 //< Scaling pixel clock delay
#define OV7670_REG_BD50MAX 0xA5            //< 50 Hz banding step limit
#define OV7670_REG_HAECC3 0xA6             //< Histogram-based AEC/AGC ctrl 3
#define OV7670_REG_HAECC4 0xA7             //< Histogram-based AEC/AGC ctrl 4
#define OV7670_REG_HAECC5 0xA8             //< Histogram-based AEC/AGC ctrl 5
#define OV7670_REG_HAECC6 0xA9             //< Histogram-based AEC/AGC ctrl 6
#define OV7670_REG_HAECC7 0xAA             //< Histogram-based AEC/AGC ctrl 7
#define OV7670_REG_BD60MAX 0xAB            //< 60 Hz banding step limit
#define OV7670_REG_ABLC1 0xB1              //< ABLC enable
#define OV7670_REG_THL_ST 0xB3             //< ABLC target
#define OV7670_REG_SATCTR 0xC9             //< Saturation control

#define OV7670_REG_LAST OV7670_REG_SATCTR //< Maximum register address

// Window Output
#define HSTART               0x17
#define HSTART_VALUE_DEFAULT 0x11
#define HSTART_VALUE_VGA     0x13
#define HSTART_VALUE_QVGA    0x13
#define HSTART_VALUE_QQVGA   0x13   // Works

#define HSTOP                0x18
#define HSTOP_VALUE_DEFAULT  0x61
#define HSTOP_VALUE_VGA      0x01
#define HSTOP_VALUE_QVGA     0x01
#define HSTOP_VALUE_QQVGA    0x01   // Works 

#define HREF                  0x32
#define HREF_VALUE_DEFAULT    0x80
#define HREF_VALUE_VGA        0xB6
#define HREF_VALUE_QVGA       0x24
#define HREF_VALUE_QQVGA      0xA4

#define VSTRT                0x19
#define VSTRT_VALUE_DEFAULT  0x03
#define VSTRT_VALUE_VGA      0x02
#define VSTRT_VALUE_QVGA     0x02
#define VSTRT_VALUE_QQVGA    0x02

#define VSTOP                0x1A
#define VSTOP_VALUE_DEFAULT  0x7B
#define VSTOP_VALUE_VGA      0x7A
#define VSTOP_VALUE_QVGA     0x7A
#define VSTOP_VALUE_QQVGA    0x7A

#define VREF                 0x03
#define VREF_VALUE_DEFAULT   0x03
#define VREF_VALUE_VGA       0x0A
#define VREF_VALUE_QVGA      0x0A
#define VREF_VALUE_QQVGA     0x0A


/* Registers */
#define REG_GAIN  0x00  /* Gain lower 8 bits (rest in vref) */
#define REG_BLUE  0x01  /* blue gain */
#define REG_RED   0x02  /* red gain */
#define REG_VREF  0x03  /* Pieces of GAIN, VSTART, VSTOP */
#define REG_COM1  0x04  /* Control 1 */
#define  COM1_CCIR656   0x40  /* CCIR656 enable */
#define REG_BAVE  0x05  /* U/B Average level */
#define REG_GbAVE 0x06  /* Y/Gb Average level */
#define REG_AECHH 0x07  /* AEC MS 5 bits */
#define REG_RAVE  0x08  /* V/R Average level */
#define REG_COM2  0x09  /* Control 2 */
#define  COM2_SSLEEP    0x10  /* Soft sleep mode */
#define REG_PID   0x0a  /* Product ID MSB */
#define REG_VER   0x0b  /* Product ID LSB */
#define REG_COM3  0x0c  /* Control 3 */
#define  COM3_SWAP    0x40    /* Byte swap */
#define  COM3_SCALEEN   0x08    /* Enable scaling */
#define  COM3_DCWEN   0x04    /* Enable downsamp/crop/window */
#define REG_COM4  0x0d  /* Control 4 */
#define REG_COM5  0x0e  /* All "reserved" */
#define REG_COM6  0x0f  /* Control 6 */
#define REG_AECH  0x10  /* More bits of AEC value */
#define REG_CLKRC 0x11  /* Clocl control */
#define   CLK_EXT   0x40    /* Use external clock directly */
#define   CLK_SCALE   0x3f    /* Mask for internal clock scale */
#define REG_COM7  0x12  /* Control 7 */
#define   COM7_RESET    0x80    /* Register reset */
#define   COM7_FMT_MASK   0x38
#define   COM7_FMT_VGA    0x00
#define   COM7_FMT_CIF    0x20    /* CIF format */
#define   COM7_FMT_QVGA   0x10    /* QVGA format */
#define   COM7_FMT_QCIF   0x08    /* QCIF format */
#define   COM7_RGB    0x04    /* bits 0 and 2 - RGB format */
#define   COM7_YUV    0x00    /* YUV */
#define   COM7_BAYER    0x01    /* Bayer format */
#define   COM7_PBAYER   0x05    /* "Processed bayer" */
#define REG_COM8  0x13  /* Control 8 */
#define   COM8_FASTAEC    0x80    /* Enable fast AGC/AEC */
#define   COM8_AECSTEP    0x40    /* Unlimited AEC step size */
#define   COM8_BFILT    0x20    /* Band filter enable */
#define   COM8_AGC    0x04    /* Auto gain enable */
#define   COM8_AWB    0x02    /* White balance enable */
#define   COM8_AEC    0x01    /* Auto exposure enable */
#define REG_COM9  0x14  /* Control 9  - gain ceiling */
#define REG_COM10 0x15  /* Control 10 */
#define   COM10_HSYNC   0x40    /* HSYNC instead of HREF */
#define   COM10_PCLK_HB   0x20    /* Suppress PCLK on horiz blank */
#define   COM10_HREF_REV  0x08    /* Reverse HREF */
#define   COM10_VS_LEAD   0x04    /* VSYNC on clock leading edge */
#define   COM10_VS_NEG    0x02    /* VSYNC negative */
#define   COM10_HS_NEG    0x01    /* HSYNC negative */
#define REG_HSTART  0x17  /* Horiz start high bits */
#define REG_HSTOP 0x18  /* Horiz stop high bits */
#define REG_VSTART  0x19  /* Vert start high bits */
#define REG_VSTOP 0x1a  /* Vert stop high bits */
#define REG_PSHFT 0x1b  /* Pixel delay after HREF */
#define REG_MIDH  0x1c  /* Manuf. ID high */
#define REG_MIDL  0x1d  /* Manuf. ID low */
#define REG_MVFP  0x1e  /* Mirror / vflip */
#define   MVFP_MIRROR   0x20    /* Mirror image */
#define   MVFP_FLIP   0x10    /* Vertical flip */

#define REG_AEW   0x24  /* AGC upper limit */
#define REG_AEB   0x25  /* AGC lower limit */
#define REG_VPT   0x26  /* AGC/AEC fast mode op region */
#define REG_HSYST 0x30  /* HSYNC rising edge delay */
#define REG_HSYEN 0x31  /* HSYNC falling edge delay */
#define REG_HREF  0x32  /* HREF pieces */
#define REG_TSLB  0x3a  /* lots of stuff */
#define   TSLB_YLAST    0x04    /* UYVY or VYUY - see com13 */
#define REG_COM11 0x3b  /* Control 11 */
#define   COM11_NIGHT   0x80    /* NIght mode enable */
#define   COM11_NMFR    0x60    /* Two bit NM frame rate */
#define   COM11_HZAUTO    0x10    /* Auto detect 50/60 Hz */
#define   COM11_50HZ    0x08    /* Manual 50Hz select */
#define   COM11_EXP   0x02
#define REG_COM12 0x3c  /* Control 12 */
#define   COM12_HREF    0x80    /* HREF always */
#define REG_COM13 0x3d  /* Control 13 */
#define   COM13_GAMMA   0x80    /* Gamma enable */
#define   COM13_UVSAT   0x40    /* UV saturation auto adjustment */
#define   COM13_UVSWAP    0x01    /* V before U - w/TSLB */
#define REG_COM14 0x3e  /* Control 14 */
#define   COM14_DCWEN   0x10    /* DCW/PCLK-scale enable */
#define REG_EDGE  0x3f  /* Edge enhancement factor */
#define REG_COM15 0x40  /* Control 15 */
#define   COM15_R10F0   0x00    /* Data range 10 to F0 */
#define   COM15_R01FE   0x80    /*            01 to FE */
#define   COM15_R00FF   0xc0    /*            00 to FF */
#define   COM15_RGB565    0x10    /* RGB565 output */
#define   COM15_RGB555    0x30    /* RGB555 output */
#define REG_COM16 0x41  /* Control 16 */
#define   COM16_AWBGAIN   0x08    /* AWB gain enable */
#define REG_COM17 0x42  /* Control 17 */
#define   COM17_AECWIN    0xc0    /* AEC window - must match COM4 */
#define   COM17_CBAR    0x08    /* DSP Color bar */

/*
 * This matrix defines how the colors are generated, must be
 * tweaked to adjust hue and saturation.
 *
 * Order: v-red, v-green, v-blue, u-red, u-green, u-blue
 *
 * They are nine-bit signed quantities, with the sign bit
 * stored in 0x58.  Sign for v-red is bit 0, and up from there.
 */
#define REG_CMATRIX_BASE 0x4f
#define   CMATRIX_LEN 6
#define REG_CMATRIX_SIGN 0x58


#define REG_BRIGHT  0x55  /* Brightness */
#define REG_CONTRAS 0x56  /* Contrast control */

#define REG_GFIX  0x69  /* Fix gain control */

#define REG_DBLV  0x6b  /* PLL control an debugging */
#define   DBLV_BYPASS   0x0a    /* Bypass PLL */
#define   DBLV_X4   0x4a    /* clock x4 */
#define   DBLV_X6   0x8a    /* clock x6 */
#define   DBLV_X8   0xca    /* clock x8 */

#define REG_SCALING_XSC 0x70  /* Test pattern and horizontal scale factor */
#define   TEST_PATTTERN_0 0x80
#define REG_SCALING_YSC 0x71  /* Test pattern and vertical scale factor */
#define   TEST_PATTTERN_1 0x80

#define REG_REG76 0x76  /* OV's name */
#define   R76_BLKPCOR   0x80    /* Black pixel correction enable */
#define   R76_WHTPCOR   0x40    /* White pixel correction enable */

#define REG_RGB444  0x8c  /* RGB 444 control */
#define   R444_ENABLE   0x02    /* Turn on RGB444, overrides 5x5 */
#define   R444_RGBX   0x01    /* Empty nibble at end */

#define REG_HAECC1  0x9f  /* Hist AEC/AGC control 1 */
#define REG_HAECC2  0xa0  /* Hist AEC/AGC control 2 */

#define REG_BD50MAX 0xa5  /* 50hz banding step limit */
#define REG_HAECC3  0xa6  /* Hist AEC/AGC control 3 */
#define REG_HAECC4  0xa7  /* Hist AEC/AGC control 4 */
#define REG_HAECC5  0xa8  /* Hist AEC/AGC control 5 */
#define REG_HAECC6  0xa9  /* Hist AEC/AGC control 6 */
#define REG_HAECC7  0xaa  /* Hist AEC/AGC control 7 */
#define REG_BD60MAX 0xab  /* 60hz banding step limit */

void writeCameraRegister(byte registerId, byte registerValue) {
  Wire.beginTransmission(CAMERA_ADDRESS);
  Wire.write(registerId);
  Wire.write(registerValue);
  Wire.endTransmission();
  delay(1);
}


void initializeCamera() {
  // these are common registers values for initializing the camera to QVGA (320 x 240) mode at the maximum clock prescaler:
  writeCameraRegister(0x12, 0x80); // reset all camera registers to default value
  delay(1000); // wait for reset proccess to be done (because it is quite slow)
  writeCameraRegister(0x3A, 0x04); //REG_TSLB
  writeCameraRegister(0x12, 0x00); //COM7
  writeCameraRegister(0x17, 0x13); //HSTART
  writeCameraRegister(0x18, 0x01); //HSTOP
  writeCameraRegister(0x32, 0xB6); //HREF
  writeCameraRegister(0x19, 0x02); //VSTRT
  writeCameraRegister(0x1A, 0x7A); //VSTOP
  writeCameraRegister(0x03, 0x0A); //VREF
  writeCameraRegister(0x0C, 0x00); //COM3 <-- formato de la imagen
  writeCameraRegister(0x3E, 0x00); //COM14
  writeCameraRegister(0x70, 0x3A); //SCALING_XSC
  writeCameraRegister(0x71, 0x35); //SCALING_YSC
  writeCameraRegister(0x72, 0x11); //SCALING_DCWCTR
  writeCameraRegister(0x73, 0xF0); //SCALING_PCLK_DIV
  writeCameraRegister(0xA2, 0x01); //SCALING_PCLK_DELAY <-- formato de la imagen has aquí
  writeCameraRegister(0x15, 0x00);
  writeCameraRegister(0x7A, 0x20);
  writeCameraRegister(0x7B, 0x10);
  writeCameraRegister(0x7C, 0x1E);
  writeCameraRegister(0x7D, 0x35);
  writeCameraRegister(0x7E, 0x5A);
  writeCameraRegister(0x7F, 0x69);
  writeCameraRegister(0x80, 0x76);
  writeCameraRegister(0x81, 0x80);
  writeCameraRegister(0x82, 0x88);
  writeCameraRegister(0x83, 0x8F);
  writeCameraRegister(0x84, 0x96);
  writeCameraRegister(0x85, 0xA3);
  writeCameraRegister(0x86, 0xAF);
  writeCameraRegister(0x87, 0xC4);
  writeCameraRegister(0x88, 0xD7);
  writeCameraRegister(0x89, 0xE8);
  writeCameraRegister(0x13, 0xC0);
  writeCameraRegister(0x00, 0x00);
  writeCameraRegister(0x10, 0x00);
  writeCameraRegister(0x0D, 0x40);
  writeCameraRegister(0x14, 0x18);
  writeCameraRegister(0xA5, 0x05);
  writeCameraRegister(0xAB, 0x07);
  writeCameraRegister(0x24, 0x95);
  writeCameraRegister(0x25, 0x33);
  writeCameraRegister(0x26, 0xE3);
  writeCameraRegister(0x9F, 0x78);
  writeCameraRegister(0xA0, 0x68);
  writeCameraRegister(0xA1, 0x03);
  writeCameraRegister(0xA6, 0xD8);
  writeCameraRegister(0xA7, 0xD8);
  writeCameraRegister(0xA8, 0xF0);
  writeCameraRegister(0xA9, 0x90);
  writeCameraRegister(0xAA, 0x94);
  writeCameraRegister(0x13, 0xC5);
  writeCameraRegister(0x30, 0x00);
  writeCameraRegister(0x31, 0x00);
  writeCameraRegister(0x0E, 0x61);
  writeCameraRegister(0x0F, 0x4B);
  writeCameraRegister(0x16, 0x02);
  writeCameraRegister(0x1E, 0x07);
  writeCameraRegister(0x21, 0x02);
  writeCameraRegister(0x22, 0x91);
  writeCameraRegister(0x29, 0x07);
  writeCameraRegister(0x33, 0x0B);
  writeCameraRegister(0x35, 0x0B);
  writeCameraRegister(0x37, 0x1D);
  writeCameraRegister(0x38, 0x71);
  writeCameraRegister(0x39, 0x2A);
  writeCameraRegister(0x3C, 0x78);
  writeCameraRegister(0x4D, 0x40);
  writeCameraRegister(0x4E, 0x20);
  writeCameraRegister(0x69, 0x00);
  writeCameraRegister(0x74, 0x10);
  writeCameraRegister(0x8D, 0x4F);
  writeCameraRegister(0x8E, 0x00);
  writeCameraRegister(0x8F, 0x00);
  writeCameraRegister(0x90, 0x00);
  writeCameraRegister(0x91, 0x00);
  writeCameraRegister(0x96, 0x00);
  writeCameraRegister(0x9A, 0x00);
  writeCameraRegister(0xB0, 0x84);
  writeCameraRegister(0xB1, 0x0C);
  writeCameraRegister(0xB2, 0x0E);
  writeCameraRegister(0xB3, 0x82);
  writeCameraRegister(0xB8, 0x0A);
  writeCameraRegister(0x43, 0x0A);
  writeCameraRegister(0x44, 0xF0);
  writeCameraRegister(0x45, 0x34);
  writeCameraRegister(0x46, 0x58);
  writeCameraRegister(0x47, 0x28);
  writeCameraRegister(0x48, 0x3A);
  writeCameraRegister(0x59, 0x88);
  writeCameraRegister(0x5A, 0x88);
  writeCameraRegister(0x5B, 0x44);
  writeCameraRegister(0x5C, 0x67);
  writeCameraRegister(0x5D, 0x49);
  writeCameraRegister(0x5E, 0x0E);
  writeCameraRegister(0x6C, 0x0A);
  writeCameraRegister(0x6D, 0x55);
  writeCameraRegister(0x6E, 0x11);
  writeCameraRegister(0x6F, 0x9E);
  writeCameraRegister(0x6A, 0x40);
  writeCameraRegister(0x01, 0x40);
  writeCameraRegister(0x02, 0x60);
  writeCameraRegister(0x13, 0xC7);
  writeCameraRegister(0x4F, 0x80); //MTX1 <- color matrix
  writeCameraRegister(0x50, 0x80);
  writeCameraRegister(0x51, 0x00);
  writeCameraRegister(0x52, 0x22);
  writeCameraRegister(0x53, 0x5E);
  writeCameraRegister(0x54, 0x80); //MTX6 <- color matrix hasta aquí
  writeCameraRegister(0x58, 0x9E);
  writeCameraRegister(0x41, 0x08);
  writeCameraRegister(0x3F, 0x00);
  writeCameraRegister(0x75, 0x05);
  writeCameraRegister(0x76, 0xE1);
  writeCameraRegister(0x4C, 0x00);
  writeCameraRegister(0x77, 0x01);
  writeCameraRegister(0x3D, 0x48); //COM13 // YUYV
  writeCameraRegister(0x4B, 0x09); //REG4B
  writeCameraRegister(0xC9, 0x60);
  writeCameraRegister(0x56, 0x40);
  writeCameraRegister(0x34, 0x11);
  writeCameraRegister(0x3B, 0x12); //COM11
  writeCameraRegister(0xA4, 0x82);
  writeCameraRegister(0x96, 0x00);
  writeCameraRegister(0x97, 0x30);
  writeCameraRegister(0x98, 0x20);
  writeCameraRegister(0x99, 0x30);
  writeCameraRegister(0x9A, 0x84);
  writeCameraRegister(0x9B, 0x29);
  writeCameraRegister(0x9C, 0x03);
  writeCameraRegister(0x9D, 0x4C);
  writeCameraRegister(0x9E, 0x3F);
  writeCameraRegister(0x78, 0x04); // que es esto
  writeCameraRegister(0x79, 0x01);
  writeCameraRegister(0xC8, 0xF0);
  writeCameraRegister(0x79, 0x0F);
  writeCameraRegister(0xC8, 0x00);
  writeCameraRegister(0x79, 0x10);
  writeCameraRegister(0xC8, 0x7E);
  writeCameraRegister(0x79, 0x0A);
  writeCameraRegister(0xC8, 0x80);
  writeCameraRegister(0x79, 0x0B);
  writeCameraRegister(0xC8, 0x01);
  writeCameraRegister(0x79, 0x0C);
  writeCameraRegister(0xC8, 0x0F);
  writeCameraRegister(0x79, 0x0D);
  writeCameraRegister(0xC8, 0x20);
  writeCameraRegister(0x79, 0x09);
  writeCameraRegister(0xC8, 0x80);
  writeCameraRegister(0x79, 0x02);
  writeCameraRegister(0xC8, 0xC0);
  writeCameraRegister(0x79, 0x03);
  writeCameraRegister(0xC8, 0x40);
  writeCameraRegister(0x79, 0x05);
  writeCameraRegister(0xC8, 0x30);
  writeCameraRegister(0x79, 0x26);
  writeCameraRegister(0xFF, 0xFF); // que es esto
  writeCameraRegister(0x15, 0x20);
  writeCameraRegister(0x0C, 0x04); //COM3
  writeCameraRegister(0x3E, 0x19); //COM14
  writeCameraRegister(0x72, 0x11); //DCWCTR
  writeCameraRegister(0x73, 0xF1); //PCLK_DIV
  writeCameraRegister(0x17, 0x16); //HSTART
  writeCameraRegister(0x18, 0x04); //HSTOP
  writeCameraRegister(0x32, 0xA4); //HREF
  writeCameraRegister(0x19, 0x02); //VSTRT
  writeCameraRegister(0x1A, 0x7A); //VSTOP
  writeCameraRegister(0x03, 0x0A); //VREF
  writeCameraRegister(0xFF, 0xFF);
  writeCameraRegister(0x12, 0x00); //COM7
  writeCameraRegister(0x8C, 0x00); //RGB444
  writeCameraRegister(0x04, 0x00); //COM1
  writeCameraRegister(0x40, 0xC0); //COM15
  writeCameraRegister(0x14, 0x6A); //COM9
  writeCameraRegister(0x4F, 0x80); //MTX1
  writeCameraRegister(0x50, 0x80); //MTX2
  writeCameraRegister(0x51, 0x00);
  writeCameraRegister(0x52, 0x22);
  writeCameraRegister(0x53, 0x5E);
  writeCameraRegister(0x54, 0x80); //MTX6
  writeCameraRegister(0x3D, 0x40); //COM13
  writeCameraRegister(0xFF, 0xFF);
  writeCameraRegister(0x11, 0x1F); //CLKRC
  writeCameraRegister(0x0C, 0x08); //COM3
  writeCameraRegister(0x3E, 0x19); //COM14
  writeCameraRegister(0x73, 0xF1); //PCLK_DIV
  writeCameraRegister(0x12, 0x10); //COM7
  delay(1000); // wait for registers to be set (because it is quite slow)
}
