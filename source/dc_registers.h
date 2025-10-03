/*
 *   This file is part of OpenDMCUFirm
 */
#ifndef _DMCU_DC_REGISTERS_H_
#define _DMCU_DC_REGISTERS_H_

#include <stdint.h>

// Note:
//  All registers are shifted right by 2 to allow passing them directly to
//  the display/graphics controller interface.

// DMCU
#define DMCU_INTERRUPT_STATUS 0x1614
#    define ABM1_HG_READY_INT_OCCURRED (1ul << 0)
#    define ABM1_HG_READY_INT_CLEAR (1ul << 0)
#    define ABM1_LS_READY_INT_OCCURRED (1ul << 1)
#    define ABM1_LS_READY_INT_CLEAR (1ul << 1)
#    define ABM1_BL_UPDATE_INT_OCCURRED (1ul << 2)
#    define ABM1_BL_UPDATE_INT_CLEAR (1ul << 2)
#    define MCP_INT_OCCURRED (1ul << 3)
#    define PM_PWR_UP_INT_OCCURRED (1ul << 4)
#    define PM_PWR_UP_INT_CLEAR (1ul << 4)
#    define PM_PWR_DOWN_INT_OCCURRED (1ul << 5)
#    define PM_PWR_DOWN_INT_CLEAR (1ul << 5)
#    define EXTERNAL_SW_INT_OCCURRED (1ul << 8)
#    define EXTERNAL_SW_INT_CLEAR (1ul << 8)
#    define SCP_INT_OCCURRED (1ul << 9)
#    define UC_INTERNAL_INT_OCCURRED (1ul << 10)
#    define UC_INTERNAL_INT_CLEAR (1ul << 10)
#    define UC_REG_RD_TIMEOUT_INT_OCCURRED (1ul << 11)
#    define UC_REG_RD_TIMEOUT_INT_CLEAR (1ul << 11)
#    define VBLANK1_INT_OCCURRED (1ul << 24)
#    define VBLANK1_INT_CLEAR (1ul << 24)
#    define VBLANK2_INT_OCCURRED (1ul << 25)
#    define VBLANK2_INT_CLEAR (1ul << 25)
#    define VBLANK3_INT_OCCURRED (1ul << 26)
#    define VBLANK3_INT_CLEAR (1ul << 26)
#    define VBLANK4_INT_OCCURRED (1ul << 27)
#    define VBLANK4_INT_CLEAR (1ul << 27)
#    define VBLANK5_INT_OCCURRED (1ul << 28)
#    define VBLANK5_INT_CLEAR (1ul << 28)
#    define VBLANK6_INT_OCCURRED (1ul << 29)
#    define VBLANK6_INT_CLEAR (1ul << 29)

#define DMCU_INTERRUPT_TO_HOST_EN_MASK 0x1615
#    define ABM1_HG_READY_INT_MASK (1ul << 0)
#    define ABM1_LS_READY_INT_MASK (1ul << 1)
#    define ABM1_BL_UPDATE_INT_MASK (1ul << 2)
#    define SCP_INT_MASK (1ul << 9)
#    define UC_INTERNAL_INT_MASK (1ul << 10)
#    define UC_REG_RD_TIMEOUT_INT_MASK (1ul << 11)

#define DMCU_INTERRUPT_TO_UC_EN_MASK 0x1616
#    define ABM1_HG_READY_INT_TO_UC_EN (1ul << 0)
#    define ABM1_LS_READY_INT_TO_UC_EN (1ul << 1)
#    define ABM1_BL_UPDATE_INT_TO_UC_EN (1ul << 2)
#    define MCP_INT_TO_UC_EN (1ul << 3)
#    define PM_PWR_UP_INT_TO_UC_EN (1ul << 4)
#    define PM_PWR_DOWN_INT_TO_UC_EN (1ul << 5)
#    define EXTERNAL_SW_INT_TO_UC_EN (1ul << 8)
#    define VBLANK1_INT_TO_UC_EN (1ul << 24)
#    define VBLANK2_INT_TO_UC_EN (1ul << 25)
#    define VBLANK3_INT_TO_UC_EN (1ul << 26)
#    define VBLANK4_INT_TO_UC_EN (1ul << 27)
#    define VBLANK5_INT_TO_UC_EN (1ul << 28)
#    define VBLANK6_INT_TO_UC_EN (1ul << 29)

#define DMCU_INTERRUPT_TO_UC_XIRQ_IRQ_SEL 0x1617
#    define ABM1_HG_READY_INT_XIRQ_IRQ_SEL (1ul << 0)
#    define ABM1_LS_READY_INT_XIRQ_IRQ_SEL (1ul << 1)
#    define ABM1_BL_UPDATE_INT_XIRQ_IRQ_SEL (1ul << 2)
#    define MCP_INT_XIRQ_IRQ_SEL (1ul << 3)
#    define PM_PWR_UP_INT_XIRQ_IRQ_SEL (1ul << 4)
#    define PM_PWR_DOWN_INT_XIRQ_IRQ_SEL (1ul << 5)
#    define EXTERNAL_SW_INT_XIRQ_IRQ_SEL (1ul << 8)
#    define VBLANK1_INT_XIRQ_IRQ_SEL (1ul << 24)
#    define VBLANK2_INT_XIRQ_IRQ_SEL (1ul << 25)
#    define VBLANK3_INT_XIRQ_IRQ_SEL (1ul << 26)
#    define VBLANK4_INT_XIRQ_IRQ_SEL (1ul << 27)
#    define VBLANK5_INT_XIRQ_IRQ_SEL (1ul << 28)
#    define VBLANK6_INT_XIRQ_IRQ_SEL (1ul << 29)

// DEINT
#define DEINT_CONFIG 0x1628
#    define DEINT_BLK_EN (1ul << 0)
#    define DEINT_KERNEL_EN (1ul << 1)
#    define Y_CBCR_SWAP (1ul << 2)
#    define CBCR_SWAP (1ul << 3)
#    define CBCR_UPSAMPLE_TYPE (1ul << 4)
#    define DEINT_VI_CAPTURE_MODE_MASK 0x3
#    define DEINT_VI_CAPTURE_MODE_SHIFT 5
#    define DEINT_VI_CAPTURE_MODE(x) ((uint32_t) (x) << DEINT_VI_CAPTURE_MODE_SHIFT)
#    define DEINT_SOFT_RESET_EN (1ul << 7)
#    define DEINT_SPATIAL_TYPE_MASK 0x7
#    define DEINT_SPATIAL_TYPE_SHIFT 8
#    define DEINT_SPATIAL_TYPE(x) ((uint32_t) (x) << DEINT_SPATIAL_TYPE_SHIFT)
#    define DEINT_ELA5_USE_MEDIAN (1ul << 11)
#    define DEINT_MAD_EN (1ul << 12)
#    define DEINT_INSERT_BLACK_BARS (1ul << 13)
#    define DEINT_HSYNC_ACTIVE_HIGH (1ul << 14)
#    define DEINT_VSYNC_ACTIVE_HIGH (1ul << 15)
#    define DEINT_MAD_INIT_WAIT_DIS (1ul << 16)
#    define DEINT_MAD_EDGE_FIX_EN (1ul << 17)
#    define DEINT_OUT_SWAP_CB_CR (1ul << 18)

#define DEINT_IF_TIM_RESOLUTION 0x1630
#    define DEINT_IF_TIM_RES_WIDTH_MASK 0x7FF
#    define DEINT_IF_TIM_RES_WIDTH_SHIFT 0
#    define DEINT_IF_TIM_RES_WIDTH(x) ((uint32_t) (x) << DEINT_IF_TIM_RES_WIDTH_SHIFT)
#    define DEINT_IF_TIM_RES_HEIGHT_MASK 0x7FF
#    define DEINT_IF_TIM_RES_HEIGHT_SHIFT 16
#    define DEINT_IF_TIM_RES_HEIGHT(x) ((uint32_t) (x) << DEINT_IF_TIM_RES_HEIGHT_SHIFT)

#define DEINT_IF_TIM_TOP_PORCH 0x1631
#    define DEINT_IF_TIM_TOP_PORCH_ODD_MASK 0x7FF
#    define DEINT_IF_TIM_TOP_PORCH_ODD_SHIFT 0
#    define DEINT_IF_TIM_TOP_PORCH_ODD(x) ((uint32_t) (x) << DEINT_IF_TIM_TOP_PORCH_ODD_SHIFT)
#    define DEINT_IF_TIM_TOP_PORCH_EVEN_MASK 0x7FF
#    define DEINT_IF_TIM_TOP_PORCH_EVEN_SHIFT 16
#    define DEINT_IF_TIM_TOP_PORCH_EVEN(x) ((uint32_t) (x) << DEINT_IF_TIM_TOP_PORCH_EVEN_SHIFT)

#define DEINT_IF_TIM_HBLANK 0x1632
#    define DEINT_IF_TIM_PRE_HBLANK_MASK 0x7FF
#    define DEINT_IF_TIM_PRE_HBLANK_SHIFT 0
#    define DEINT_IF_TIM_PRE_HBLANK(x) ((uint32_t) (x) << DEINT_IF_TIM_PRE_HBLANK_SHIFT)
#    define DEINT_IF_TIM_POST_HBLANK_MASK 0x7FF
#    define DEINT_IF_TIM_POST_HBLANK_SHIFT 16
#    define DEINT_IF_TIM_POST_HBLANK(x) ((uint32_t) (x) << DEINT_IF_TIM_POST_HBLANK_SHIFT)

#define DEINT_RSVD2 0x163A

// CRTC
#define D1CRTC_H_TOTAL 0x1800
#    define CRTC_H_TOTAL_MASK 0x1FFF
#    define CRTC_H_TOTAL_SHIFT 0
#    define CRTC_H_TOTAL(x) ((uint32_t) (x) << CRTC_H_TOTAL_SHIFT)

#define D1CRTC_H_BLANK_START_END 0x1801
#    define CRTC_H_BLANK_START_MASK 0x1FFF
#    define CRTC_H_BLANK_START_SHIFT 0
#    define CRTC_H_BLANK_START(x) ((uint32_t) (x) << CRTC_H_BLANK_START_SHIFT)
#    define CRTC_H_BLANK_END_MASK 0x1FFF
#    define CRTC_H_BLANK_END_SHIFT 16
#    define CRTC_H_BLANK_END(x) ((uint32_t) (x) << CRTC_H_BLANK_END_SHIFT)

#define D1CRTC_H_SYNC_A 0x1802
#    define CRTC_H_SYNC_A_START_MASK 0x1FFF
#    define CRTC_H_SYNC_A_START_SHIFT 0
#    define CRTC_H_SYNC_A_START(x) ((uint32_t) (x) << CRTC_H_SYNC_A_START_SHIFT)
#    define CRTC_H_SYNC_A_END_MASK 0x1FFF
#    define CRTC_H_SYNC_A_END_SHIFT 16
#    define CRTC_H_SYNC_A_END(x) ((uint32_t) (x) << CRTC_H_SYNC_A_END_SHIFT)

#define D1CRTC_V_TOTAL 0x1808
#    define CRTC_V_TOTAL_MASK 0x1FFF
#    define CRTC_V_TOTAL_SHIFT 0
#    define CRTC_V_TOTAL(x) ((uint32_t) (x) << CRTC_V_TOTAL_SHIFT)

#define D1CRTC_V_BLANK_START_END 0x1809
#    define CRTC_V_BLANK_START_MASK 0x1FFF
#    define CRTC_V_BLANK_START_SHIFT 0
#    define CRTC_V_BLANK_START(x) ((uint32_t) (x) << CRTC_V_BLANK_START_SHIFT)
#    define CRTC_V_BLANK_END_MASK 0x1FFF
#    define CRTC_V_BLANK_END_SHIFT 16
#    define CRTC_V_BLANK_END(x) ((uint32_t) (x) << CRTC_V_BLANK_END_SHIFT)

#define D1CRTC_V_SYNC_A 0x180A
#    define CRTC_V_SYNC_A_START_MASK 0x1FFF
#    define CRTC_V_SYNC_A_START_SHIFT 0
#    define CRTC_V_SYNC_A_START(x) ((uint32_t) (x) << CRTC_V_SYNC_A_START_SHIFT)
#    define CRTC_V_SYNC_A_END_MASK 0x1FFF
#    define CRTC_V_SYNC_A_END_SHIFT 16
#    define CRTC_V_SYNC_A_END(x) ((uint32_t) (x) << CRTC_V_SYNC_A_END_SHIFT)

#define D1CRTC_CONTROL 0x1820
#    define CRTC_MASTER_EN (1ul << 0)
#    define CRTC_SYNC_RESET_SEL (1ul << 4)
#    define CRTC_DISABLE_POINT_CNTL_MASK 0x3
#    define CRTC_DISABLE_POINT_CNTL_SHIFT 8
#    define CRTC_DISABLE_POINT_CNTL(x) ((uint32_t) (x) << CRTC_DISABLE_POINT_CNTL_SHIFT)
#    define CRTC_START_POINT_CNTL (1ul << 12)
#    define CRTC_FIELD_NUMBER_CNTL (1ul << 13)
#    define CRTC_CURRENT_MASTER_EN_STATE (1ul << 16)
#    define CRTC_HBLANK_EARLY_CONTROL_MASK 0x7
#    define CRTC_HBLANK_EARLY_CONTROL_SHIFT 20
#    define CRTC_HBLANK_EARLY_CONTROL(x) ((uint32_t) (x) << CRTC_HBLANK_EARLY_CONTROL_SHIFT)
#    define CRTC_DISP_READ_REQUEST_DISABLE (1ul << 24)
#    define CRTC_PREFETCH_EN (1ul << 28)
#    define CRTC_SOF_PULL_EN (1ul << 29)

#define D1CRTC_BLANK_CONTROL 0x1821
#    define CRTC_CURRENT_BLANK_STATE (1ul << 0)
#    define CRTC_BLANK_DATA_EN (1ul << 8)
#    define CRTC_BLANK_DE_MODE (1ul << 16)

#define D1CRTC_BLACK_COLOR 0x1826
#    define CRTC_BLACK_COLOR_B_CB_MASK 0x3FF
#    define CRTC_BLACK_COLOR_B_CB_SHIFT 0
#    define CRTC_BLACK_COLOR_B_CB(x) ((uint32_t) (x) << CRTC_BLACK_COLOR_B_CB_SHIFT)
#    define CRTC_BLACK_COLOR_G_Y_MASK 0x3FF
#    define CRTC_BLACK_COLOR_G_Y_SHIFT 10
#    define CRTC_BLACK_COLOR_G_Y(x) ((uint32_t) (x) << CRTC_BLACK_COLOR_G_Y_SHIFT)
#    define CRTC_BLACK_COLOR_R_CR_MASK 0x3FF
#    define CRTC_BLACK_COLOR_R_CR_SHIFT 20
#    define CRTC_BLACK_COLOR_R_CR(x) ((uint32_t) (x) << CRTC_BLACK_COLOR_R_CR_SHIFT)

#define D1CRTC_STATUS_POSITION 0x1828
#    define CRTC_VERT_COUNT_MASK 0x1FFF
#    define CRTC_VERT_COUNT_SHIFT 0
#    define CRTC_VERT_COUNT(x) ((uint32_t) (x) << CRTC_VERT_COUNT_SHIFT)
#    define CRTC_HORZ_COUNT_MASK 0x1FFF
#    define CRTC_HORZ_COUNT_SHIFT 16
#    define CRTC_HORZ_COUNT(x) ((uint32_t) (x) << CRTC_HORZ_COUNT_SHIFT)

#define D1CRTC_COUNT_RESET 0x182C
#    define CRTC_RESET_FRAME_COUNT (1ul << 0)

#define D1MODE_MASTER_UPDATE_LOCK 0x1838
#    define MODE_MASTER_UPDATE_LOCK (1ul << 0)

#define D1CRTC_DOUBLE_BUFFER_CONTROL 0x183B
#    define CRTC_UPDATE_PENDING (1ul << 0)
#    define CRTC_UPDATE_INSTANTLY (1ul << 8)
#    define CRTC_BLANK_DATA_DOUBLE_BUFFER_EN (1ul << 16)

#define D2CRTC_H_TOTAL 0x1A00

#define D2CRTC_H_BLANK_START_END 0x1A01

#define D2CRTC_H_SYNC_A 0x1A02

#define D2CRTC_V_TOTAL 0x1A08

#define D2CRTC_V_BLANK_START_END 0x1A09

#define D2CRTC_V_SYNC_A 0x1A0A

#define D2CRTC_CONTROL 0x1A20

#define D2CRTC_BLANK_CONTROL 0x1A21

#define D2CRTC_BLACK_COLOR 0x1A26

#define D2MODE_MASTER_UPDATE_LOCK 0x1A38

#define D2CRTC_DOUBLE_BUFFER_CONTROL 0x1A3B

// DCP
#define D1GRPH_ENABLE 0x1840
#    define GRPH_ENABLE (1ul << 0)

#define D1GRPH_PITCH 0x1848
#    define GRPH_PITCH_MASK 0x3FFF
#    define GRPH_PITCH_SHIFT 0
#    define GRPH_PITCH(x) ((uint32_t) (x) << GRPH_PITCH_SHIFT)

#define D1GRPH_X_END 0x184D
#    define GRPH_X_END_MASK 0x3FFF
#    define GRPH_X_END_SHIFT 0
#    define GRPH_X_END(x) ((uint32_t) (x) << GRPH_X_END_SHIFT)

#define D1GRPH_Y_END 0x184E
#    define GRPH_Y_END_MASK 0x3FFF
#    define GRPH_Y_END_SHIFT 0
#    define GRPH_Y_END(x) ((uint32_t) (x) << GRPH_Y_END_SHIFT)

#define D1GRPH_UPDATE 0x1851
#    define GRPH_MODE_UPDATE_PENDING (1ul << 0)
#    define GRPH_MODE_UPDATE_TAKEN (1ul << 1)
#    define GRPH_SURFACE_UPDATE_PENDING (1ul << 2)
#    define GRPH_SURFACE_UPDATE_TAKEN (1ul << 3)
#    define GRPH_UPDATE_LOCK (1ul << 16)
#    define GRPH_MODE_DISABLE_MULTIPLE_UPDATE (1ul << 24)
#    define GRPH_SURFACE_DISABLE_MULTIPLE_UPDATE (1ul << 28)

#define D2GRPH_X_END 0x1A4D

#define D2GRPH_Y_END 0x1A4E

#define D2GRPH_PITCH 0x1A48

// SCL
#define D1MODE_VIEWPORT_START 0x1960
#    define MODE_VIEWPORT_Y_START_MASK 0x1FFF
#    define MODE_VIEWPORT_Y_START_SHIFT 0
#    define MODE_VIEWPORT_Y_START(x) ((uint32_t) (x) << MODE_VIEWPORT_Y_START_SHIFT)
#    define MODE_VIEWPORT_X_START_MASK 0x1FFF
#    define MODE_VIEWPORT_X_START_SHIFT 16
#    define MODE_VIEWPORT_X_START(x) ((uint32_t) (x) << MODE_VIEWPORT_X_START_SHIFT)

#define D1MODE_VIEWPORT_SIZE 0x1961
#    define MODE_VIEWPORT_HEIGHT_MASK 0x3FFF
#    define MODE_VIEWPORT_HEIGHT_SHIFT 0
#    define MODE_VIEWPORT_HEIGHT(x) ((uint32_t) (x) << MODE_VIEWPORT_HEIGHT_SHIFT)
#    define MODE_VIEWPORT_WIDTH_MASK 0x3FFF
#    define MODE_VIEWPORT_WIDTH_SHIFT 16
#    define MODE_VIEWPORT_WIDTH(x) ((uint32_t) (x) << MODE_VIEWPORT_WIDTH_SHIFT)

#define D1MODE_EXT_OVERSCAN_LEFT_RIGHT 0x1962
#    define MODE_EXT_OVERSCAN_RIGHT_MASK 0xFFF
#    define MODE_EXT_OVERSCAN_RIGHT_SHIFT 0
#    define MODE_EXT_OVERSCAN_RIGHT(x) ((uint32_t) (x) << MODE_EXT_OVERSCAN_RIGHT_SHIFT)
#    define MODE_EXT_OVERSCAN_LEFT_MASK 0xFFF
#    define MODE_EXT_OVERSCAN_LEFT_SHIFT 16
#    define MODE_EXT_OVERSCAN_LEFT(x) ((uint32_t) (x) << MODE_EXT_OVERSCAN_LEFT_SHIFT)

#define D1SCL_SCALER_ENABLE 0x1964
#    define SCL_SCALE_EN (1ul << 0)

#define D1SCL_TAP_CONTROL 0x1965
#    define SCL_VERT_NUM_OF_TAPS_MASK 0x7
#    define SCL_VERT_NUM_OF_TAPS_SHIFT 0
#    define SCL_VERT_NUM_OF_TAPS(x) ((uint32_t) (x) << SCL_VERT_NUM_OF_TAPS_SHIFT)
#    define SCL_HORZ_NUM_OF_TAPS_MASK 0xF
#    define SCL_HORZ_NUM_OF_TAPS_SHIFT 8
#    define SCL_HORZ_NUM_OF_TAPS(x) ((uint32_t) (x) << SCL_HORZ_NUM_OF_TAPS_SHIFT)

#define D1SCL_AUTOMATIC_MODE_CONTROL 0x1969
#    define SCL_VERT_CALC_AUTO_RATIO_EN (1ul << 0)
#    define SCL_VERT_CALC_AUTO_COEF_EN (1ul << 8)
#    define SCL_HORZ_CALC_AUTO_RATIO_EN (1ul << 16)
#    define SCL_HORZ_CALC_AUTO_COEF_EN (1ul << 24)

#define D1SCL_HORZ_FILTER_CONTROL 0x196C
#    define SCL_H_FILTER_PICK_NEAREST (1ul << 0)
#    define SCL_H_2TAP_ALPHA_COEF_EN (1ul << 8)
#    define SCL_H_2TAP_ALPHA_COEF_MODE_MASK 0x3
#    define SCL_H_2TAP_ALPHA_COEF_MODE_SHIFT 16
#    define SCL_H_2TAP_ALPHA_COEF_MODE(x) ((uint32_t) (x) << SCL_H_2TAP_ALPHA_COEF_MODE_SHIFT)

#define D1SCL_VERT_FILTER_CONTROL 0x1970
#    define SCL_V_FILTER_PICK_NEAREST (1ul << 0)
#    define SCL_V_2TAP_ALPHA_COEF_EN (1ul << 8)
#    define SCL_V_2TAP_ALPHA_COEF_MODE_MASK 0x3
#    define SCL_V_2TAP_ALPHA_COEF_MODE_SHIFT 16
#    define SCL_V_2TAP_ALPHA_COEF_MODE(x) ((uint32_t) (x) << SCL_V_2TAP_ALPHA_COEF_MODE_SHIFT)
#    define SCL_BAND_FILTER_BYPASS (1ul << 24)

#define D2MODE_VIEWPORT_START 0x1B60

#define D2MODE_VIEWPORT_SIZE 0x1B61

#define D2MODE_EXT_OVERSCAN_LEFT_RIGHT 0x1B62

#define D2SCL_SCALER_ENABLE 0x1B64

#define D2SCL_TAP_CONTROL 0x1B65

#define D2SCL_AUTOMATIC_MODE_CONTROL 0x1B69

#define D2SCL_HORZ_FILTER_CONTROL 0x1B6C

#define D2SCL_VERT_FILTER_CONTROL 0x1B70

// DVO
#define DVOA_ENABLE 0x1C20
#    define DVO_ENABLE (1ul << 0)
#    define DVO_PIXEL_ENCODING (1ul << 8)

#define DVOA_CONFIG 0x1C26
#    define DVO_NEW_PIXEL_MAP (1ul << 0)
#    define DVO_MODE_SD_HDN (1ul << 1)
#    define DVO_FORMAT_RGB (1ul << 2)
#    define DVO_NIN (1ul << 3)
#    define DVO_VIDEO_FORMAT_MASK 0x3
#    define DVO_VIDEO_FORMAT_SHIFT 4
#    define DVO_VIDEO_FORMAT(x) ((uint32_t) (x) << DVO_VIDEO_FORMAT_SHIFT)
#    define DVO_REFRESH_RATE_EN (1ul << 8)
#    define DVO_REFRESH_RATE_FLAGS_MASK 0x3
#    define DVO_REFRESH_RATE_FLAGS_SHIFT 9
#    define DVO_REFRESH_RATE_FLAGS(x) ((uint32_t) (x) << DVO_REFRESH_RATE_FLAGS_SHIFT)
#    define DVO_SWAP_Y_CBCR (1ul << 22)
#    define DVO_SWAP_CB_CR (1ul << 23)
#    define DVO_DSYNC_POL (1ul << 24)
#    define DVO_HSYNC_POL (1ul << 25)
#    define DVO_VSYNC_POL (1ul << 26)
#    define DVO_DE_POL (1ul << 27)
#    define DVO_INTERLACE_POL (1ul << 28)
#    define DVO_NTSC_POL (1ul << 29)
#    define DVO_FIELD_POL (1ul << 30)

#define DVOB_ENABLE 0x1C30

#define DVOB_CONFIG 0x1C36

// DISPOUT
#define DOUT_SCRATCH0 0x1F00
#define DOUT_SCRATCH1 0x1F01
#define DOUT_SCRATCH2 0x1F02
#define DOUT_SCRATCH3 0x1F03
#define DOUT_SCRATCH4 0x1F04
#define DOUT_SCRATCH5 0x1F05
#define DOUT_SCRATCH6 0x1F06
#define DOUT_SCRATCH7 0x1F07

// DCIO
#define DC_GPIO_DVOADATA_MASK 0x1F8C
#    define DC_GPIO_DVODATA_MASK_MASK 0xFFFFFF
#    define DC_GPIO_DVODATA_MASK_SHIFT 0
#    define DC_GPIO_DVODATA_MASK(x) ((uint32_t) (x) << DC_GPIO_DVODATA_MASK_SHIFT)
#    define DC_GPIO_DVOCNTL_MASK_MASK 0x7
#    define DC_GPIO_DVOCNTL_MASK_SHIFT 24
#    define DC_GPIO_DVOCNTL_MASK(x) ((uint32_t) (x) << DC_GPIO_DVOCNTL_MASK_SHIFT)
#    define DC_GPIO_DVOPHASE_MASK (1ul << 27)
#    define DC_GPIO_DVOCLK_MASK (1ul << 28)
#    define DC_GPIO_MVP_DVOCNTL_MASK_MASK 0x3
#    define DC_GPIO_MVP_DVOCNTL_MASK_SHIFT 30
#    define DC_GPIO_MVP_DVOCNTL_MASK(x) ((uint32_t) (x) << DC_GPIO_MVP_DVOCNTL_MASK_SHIFT)

#define DC_GPIO_DVOBDATA_MASK 0x1FB8

#endif
