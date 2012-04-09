#ifndef REGADDR_H
#define REGADDR_H

/* Registers for sensors and LEDs in fpga logic */
#define SENSREGA    0xd3000000
#define SENSREGB    0xd3000004
#define SENSREGC    0xd3000008
#define SENSREGD    0xd300000c
#define SENSREGE    0xd3000010
#define SENSREGF    0xd3000014
#define SENSREGG    0xd3000018
#define LEDOCT3REG  0xd300001c
#define LEDOCT4REG  0xd3000020
#define LEDOCT5REG  0xd3000024

// base address for Audio codec
#define AUDIO_BASE 		0xd3000028

// offsets for Audio codec
#define RESET_AUDIO 		0x00
#define MASTER_VOL 		0x02
#define HEADPHONE_VOL 		0x04
#define MONO_VOL 		0x06
#define	PHONE_VOL 		0x0C
#define MIC_VOL 		0x0E
#define LINEIN_VOL 		0x10
#define CD_VOL 			0x12
#define AUX_VOL 		0x16
#define PCM_OUT_VOL 		0x18
#define REC_SEL 		0x1A
#define REC_GAIN 		0x1C
#define GEN_PURP_A 		0x20
#define POWER_DOWN_STAT_A 	0x26
#define EXTD_AUDIO_ID	 	0x28
#define EXTD_AUDIO_STAT 	0x2A
#define PCM_FRONT_DAC_RATE 	0x2C
#define PCM_LR_ADC_RATE 	0x32
#define SPDIF_CTRL		0x3A
#define EQ_CTRL			0x60
#define EQ_DATA 		0x62
#define MIXER_ADC_VOL 		0x64
#define JACK_SENSE 		0x72
#define SERIAL_CONF 		0x74
#define MISC_CTRL_BIT 		0x76
#define VEND_ID1 		0x7C
#define VEND_ID2 		0x7E

#endif
