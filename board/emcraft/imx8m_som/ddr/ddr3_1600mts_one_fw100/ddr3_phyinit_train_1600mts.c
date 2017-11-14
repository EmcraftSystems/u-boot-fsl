#include <common.h>
#include <asm/io.h>
#include <asm/arch/ddr_memory_map.h>
#include <asm/arch/clock.h>
#define reg32_write(a,v) writel(v,a)
#define reg32_read(v) readl(v)

#define DDR_ONE_RANK
#define RUN_ON_SILICON 

#define  reg32setbit(addr,bitpos) \
    reg32_write((addr),(reg32_read((addr)) | (1<<(bitpos))))
#define  reg16setbit(addr,bitpos) \
    reg16_write((addr),(reg16_read((addr)) | (1<<(bitpos))))
#define  reg8setbit(addr,bitpos) \
    reg8_write((addr),(reg16_read((addr)) | (1<<(bitpos))))

typedef volatile unsigned int WORD;
#define dwc_ddrphy_apb_wr(addr, data)  reg32_write(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*(addr), data)
#define dwc_ddrphy_apb_rd(addr)        (reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*(addr)))
void ddr3_phyinit_train_1600mts(WORD after_retention){
// [dwc_ddrphy_phyinit_main] Start of dwc_ddrphy_phyinit_main()
// [dwc_ddrphy_phyinit_sequence] Start of dwc_ddrphy_phyinit_sequence()
// [dwc_ddrphy_phyinit_initStruct] Start of dwc_ddrphy_phyinit_initStruct()
// [dwc_ddrphy_phyinit_initStruct] End of dwc_ddrphy_phyinit_initStruct()
// [dwc_ddrphy_phyinit_setDefault] Start of dwc_ddrphy_phyinit_setDefault()
// [dwc_ddrphy_phyinit_setDefault] End of dwc_ddrphy_phyinit_setDefault()


// //##############################################################
// 
// // dwc_ddrphy_phyinit_userCustom_overrideUserInput is a user-editable function.
// //
// // See PhyInit App Note for detailed description and function usage
//
// //##############################################################

//zhouwei  dwc_ddrphy_phyinit_userCustom_overrideUserInput ();
// 
//  [dwc_ddrphy_phyinit_userCustom_overrideUserInput] End of dwc_ddrphy_phyinit_userCustom_overrideUserInput()
//[dwc_ddrphy_phyinit_calcMb] Start of dwc_ddrphy_phyinit_calcMb()
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[0].DramType to 0x1
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[0].Pstate to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[0].DRAMFreq to 0x640
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[0].PllBypassEn to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[0].DfiFreqRatio to 0x2
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[0].PhyOdtImpedance to 0x18
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[0].PhyDrvImpedance to 0x28
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[0].BPZNResVal to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[0].EnabledDQs to 0x20
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[0].PhyCfg to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[0].DisabledDbyte to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[1].DramType to 0x1
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[1].Pstate to 0x1
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[1].DRAMFreq to 0x74a
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[1].PllBypassEn to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[1].DfiFreqRatio to 0x2
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[1].PhyOdtImpedance to 0x18
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[1].PhyDrvImpedance to 0x28
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[1].BPZNResVal to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[1].EnabledDQs to 0x20
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[1].PhyCfg to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[1].DisabledDbyte to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[2].DramType to 0x1
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[2].Pstate to 0x2
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[2].DRAMFreq to 0x74a
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[2].PllBypassEn to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[2].DfiFreqRatio to 0x2
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[2].PhyOdtImpedance to 0x18
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[2].PhyDrvImpedance to 0x28
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[2].BPZNResVal to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[2].EnabledDQs to 0x20
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[2].PhyCfg to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[2].DisabledDbyte to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[3].DramType to 0x1
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[3].Pstate to 0x3
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[3].DRAMFreq to 0x74a
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[3].PllBypassEn to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[3].DfiFreqRatio to 0x2
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[3].PhyOdtImpedance to 0x18
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[3].PhyDrvImpedance to 0x28
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[3].BPZNResVal to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[3].EnabledDQs to 0x20
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[3].PhyCfg to 0x0
// // [dwc_ddrphy_phyinit_softSetMb] Setting mb_DDR3U_1D[3].DisabledDbyte to 0x0
//[dwc_ddrphy_phyinit_calcMb] End of dwc_ddrphy_phyinit_calcMb()
// // [phyinit_print_dat] // ####################################################
// // [phyinit_print_dat] // 
// // [phyinit_print_dat] // Printing values in user input structure
// // [phyinit_print_dat] // 
// // [phyinit_print_dat] // ####################################################
// // [phyinit_print_dat] userInputBasic.Frequency[0] = 800
// // [phyinit_print_dat] userInputBasic.Frequency[1] = 933
// // [phyinit_print_dat] userInputBasic.Frequency[2] = 933
// // [phyinit_print_dat] userInputBasic.Frequency[3] = 933
// // [phyinit_print_dat] userInputBasic.NumRank_dfi0 = 2
// // [phyinit_print_dat] userInputBasic.ReadDBIEnable[0] = 0
// // [phyinit_print_dat] userInputBasic.ReadDBIEnable[1] = 0
// // [phyinit_print_dat] userInputBasic.ReadDBIEnable[2] = 0
// // [phyinit_print_dat] userInputBasic.ReadDBIEnable[3] = 0
// // [phyinit_print_dat] userInputBasic.Lp4xMode = 0
// // [phyinit_print_dat] userInputBasic.DimmType = 4
// // [phyinit_print_dat] userInputBasic.DfiMode = 0
// // [phyinit_print_dat] userInputBasic.DramType = 1
// // [phyinit_print_dat] userInputBasic.HardMacroVer = 3
// // [phyinit_print_dat] userInputBasic.DfiFreqRatio[0] = 1
// // [phyinit_print_dat] userInputBasic.DfiFreqRatio[1] = 1
// // [phyinit_print_dat] userInputBasic.DfiFreqRatio[2] = 1
// // [phyinit_print_dat] userInputBasic.DfiFreqRatio[3] = 1
// // [phyinit_print_dat] userInputBasic.NumAnib = 10
// // [phyinit_print_dat] userInputBasic.NumDbyte = 4
// // [phyinit_print_dat] userInputBasic.DramDataWidth = 16
// // [phyinit_print_dat] userInputBasic.PllBypass[0] = 0
// // [phyinit_print_dat] userInputBasic.PllBypass[1] = 0
// // [phyinit_print_dat] userInputBasic.PllBypass[2] = 0
// // [phyinit_print_dat] userInputBasic.PllBypass[3] = 0
// // [phyinit_print_dat] userInputBasic.Dfi1Exists = 1
// // [phyinit_print_dat] userInputBasic.Train2D = 0
// // [phyinit_print_dat] userInputBasic.NumRank_dfi1 = 0
// // [phyinit_print_dat] userInputBasic.NumActiveDbyteDfi0 = 4
// // [phyinit_print_dat] userInputBasic.NumPStates = 1
// // [phyinit_print_dat] userInputBasic.NumActiveDbyteDfi1 = 0
// // [phyinit_print_dat] userInputAdvanced.DisDynAdrTri[0] = 1
// // [phyinit_print_dat] userInputAdvanced.DisDynAdrTri[1] = 1
// // [phyinit_print_dat] userInputAdvanced.DisDynAdrTri[2] = 1
// // [phyinit_print_dat] userInputAdvanced.DisDynAdrTri[3] = 1
// // [phyinit_print_dat] userInputAdvanced.SnpsUmctlF0RC5x[0] = 0
// // [phyinit_print_dat] userInputAdvanced.SnpsUmctlF0RC5x[1] = 0
// // [phyinit_print_dat] userInputAdvanced.SnpsUmctlF0RC5x[2] = 0
// // [phyinit_print_dat] userInputAdvanced.SnpsUmctlF0RC5x[3] = 0
// // [phyinit_print_dat] userInputAdvanced.MemAlertEn = 1
// // [phyinit_print_dat] userInputAdvanced.DramByteSwap = 0
// // [phyinit_print_dat] userInputAdvanced.ExtCalResVal = 0
// // [phyinit_print_dat] userInputAdvanced.TxSlewRiseDQ[0] = 15
// // [phyinit_print_dat] userInputAdvanced.TxSlewRiseDQ[1] = 15
// // [phyinit_print_dat] userInputAdvanced.TxSlewRiseDQ[2] = 15
// // [phyinit_print_dat] userInputAdvanced.TxSlewRiseDQ[3] = 15
// // [phyinit_print_dat] userInputAdvanced.D4TxPreambleLength[0] = 0
// // [phyinit_print_dat] userInputAdvanced.D4TxPreambleLength[1] = 0
// // [phyinit_print_dat] userInputAdvanced.D4TxPreambleLength[2] = 0
// // [phyinit_print_dat] userInputAdvanced.D4TxPreambleLength[3] = 0
// // [phyinit_print_dat] userInputAdvanced.RxEnBackOff = 0
// // [phyinit_print_dat] userInputAdvanced.D4RxPreambleLength[0] = 0
// // [phyinit_print_dat] userInputAdvanced.D4RxPreambleLength[1] = 0
// // [phyinit_print_dat] userInputAdvanced.D4RxPreambleLength[2] = 0
// // [phyinit_print_dat] userInputAdvanced.D4RxPreambleLength[3] = 0
// // [phyinit_print_dat] userInputAdvanced.TxSlewFallDQ[0] = 15
// // [phyinit_print_dat] userInputAdvanced.TxSlewFallDQ[1] = 15
// // [phyinit_print_dat] userInputAdvanced.TxSlewFallDQ[2] = 15
// // [phyinit_print_dat] userInputAdvanced.TxSlewFallDQ[3] = 15
// // [phyinit_print_dat] userInputAdvanced.CalOnce = 0
// // [phyinit_print_dat] userInputAdvanced.PhyMstrMaxReqToAck[0] = 0
// // [phyinit_print_dat] userInputAdvanced.PhyMstrMaxReqToAck[1] = 0
// // [phyinit_print_dat] userInputAdvanced.PhyMstrMaxReqToAck[2] = 0
// // [phyinit_print_dat] userInputAdvanced.PhyMstrMaxReqToAck[3] = 0
// // [phyinit_print_dat] userInputAdvanced.TxSlewFallAC = 15
// // [phyinit_print_dat] userInputAdvanced.ATxImpedance = 20
// // [phyinit_print_dat] userInputAdvanced.Is2Ttiming[0] = 0
// // [phyinit_print_dat] userInputAdvanced.Is2Ttiming[1] = 0
// // [phyinit_print_dat] userInputAdvanced.Is2Ttiming[2] = 0
// // [phyinit_print_dat] userInputAdvanced.Is2Ttiming[3] = 0
// // [phyinit_print_dat] userInputAdvanced.MemAlertVrefLevel = 41
// // [phyinit_print_dat] userInputAdvanced.PhyMstrTrainInterval[0] = 0
// // [phyinit_print_dat] userInputAdvanced.PhyMstrTrainInterval[1] = 0
// // [phyinit_print_dat] userInputAdvanced.PhyMstrTrainInterval[2] = 0
// // [phyinit_print_dat] userInputAdvanced.PhyMstrTrainInterval[3] = 0
// // [phyinit_print_dat] userInputAdvanced.SnpsUmctlOpt = 0
// // [phyinit_print_dat] userInputAdvanced.WDQSExt = 0
// // [phyinit_print_dat] userInputAdvanced.CalInterval = 9
// // [phyinit_print_dat] userInputAdvanced.MemAlertPUImp = 5
// // [phyinit_print_dat] userInputAdvanced.ODTImpedance[0] = 24
// // [phyinit_print_dat] userInputAdvanced.ODTImpedance[1] = 24
// // [phyinit_print_dat] userInputAdvanced.ODTImpedance[2] = 24
// // [phyinit_print_dat] userInputAdvanced.ODTImpedance[3] = 24
// // [phyinit_print_dat] userInputAdvanced.TxSlewRiseAC = 15
// // [phyinit_print_dat] userInputAdvanced.TrainSequenceCtrl = 0
// // [phyinit_print_dat] userInputAdvanced.MemAlertSyncBypass = 0
// // [phyinit_print_dat] userInputAdvanced.TxImpedance[0] = 40
// // [phyinit_print_dat] userInputAdvanced.TxImpedance[1] = 40
// // [phyinit_print_dat] userInputAdvanced.TxImpedance[2] = 40
// // [phyinit_print_dat] userInputAdvanced.TxImpedance[3] = 40
// // [phyinit_print_dat] userInputSim.tDQS2DQ    = 0
// // [phyinit_print_dat] userInputSim.tDQSCK     = 0
// // [phyinit_print_dat] userInputSim.tSTAOFF[0] = 0
// // [phyinit_print_dat] userInputSim.tSTAOFF[1] = 0
// // [phyinit_print_dat] userInputSim.tSTAOFF[2] = 0
// // [phyinit_print_dat] userInputSim.tSTAOFF[3] = 0
// // [phyinit_print_dat] // ####################################################
// // [phyinit_print_dat] // 
// // [phyinit_print_dat] // Printing values of 1D message block input/inout fields, PState=0
// // [phyinit_print_dat] // 
// // [phyinit_print_dat] // ####################################################
// // [phyinit_print_dat] mb_DDR3U_1D[0].Reserved00 = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].MsgMisc = 0x6
// // [phyinit_print_dat] mb_DDR3U_1D[0].Pstate = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].PllBypassEn = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].DRAMFreq = 0x640
// // [phyinit_print_dat] mb_DDR3U_1D[0].DfiFreqRatio = 0x2
// // [phyinit_print_dat] mb_DDR3U_1D[0].BPZNResVal = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].PhyOdtImpedance = 0x18
// // [phyinit_print_dat] mb_DDR3U_1D[0].PhyDrvImpedance = 0x28
// // [phyinit_print_dat] mb_DDR3U_1D[0].PhyVref = 0x3b
// // [phyinit_print_dat] mb_DDR3U_1D[0].DramType = 0x1
// // [phyinit_print_dat] mb_DDR3U_1D[0].DisabledDbyte = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].EnabledDQs = 0x20
// // [phyinit_print_dat] mb_DDR3U_1D[0].CsPresent = 0x3
// // [phyinit_print_dat] mb_DDR3U_1D[0].CsPresentD0 = 0x3
// // [phyinit_print_dat] mb_DDR3U_1D[0].CsPresentD1 = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].AddrMirror = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].PhyCfg = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].SequenceCtrl = 0x31f
// // [phyinit_print_dat] mb_DDR3U_1D[0].HdtCtrl = 0xc8
// // [phyinit_print_dat] mb_DDR3U_1D[0].PhyConfigOverride = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].DFIMRLMargin = 0x1
// // [phyinit_print_dat] mb_DDR3U_1D[0].MR0 = 0xd70
// // [phyinit_print_dat] mb_DDR3U_1D[0].MR1 = 0x44
// // [phyinit_print_dat] mb_DDR3U_1D[0].MR2 = 0x18
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl0 = 0x21
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl1 = 0x12
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl2 = 0x84
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl3 = 0x48
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl4 = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl5 = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl6 = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl7 = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].Reserved00 = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].MsgMisc = 0x6
// // [phyinit_print_dat] mb_DDR3U_1D[0].Pstate = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].PllBypassEn = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].DRAMFreq = 0x640
// // [phyinit_print_dat] mb_DDR3U_1D[0].DfiFreqRatio = 0x2
// // [phyinit_print_dat] mb_DDR3U_1D[0].BPZNResVal = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].PhyOdtImpedance = 0x18
// // [phyinit_print_dat] mb_DDR3U_1D[0].PhyDrvImpedance = 0x28
// // [phyinit_print_dat] mb_DDR3U_1D[0].PhyVref = 0x3b
// // [phyinit_print_dat] mb_DDR3U_1D[0].DramType = 0x1
// // [phyinit_print_dat] mb_DDR3U_1D[0].DisabledDbyte = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].EnabledDQs = 0x20
// // [phyinit_print_dat] mb_DDR3U_1D[0].CsPresent = 0x3
// // [phyinit_print_dat] mb_DDR3U_1D[0].CsPresentD0 = 0x3
// // [phyinit_print_dat] mb_DDR3U_1D[0].CsPresentD1 = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].AddrMirror = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].PhyCfg = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].SequenceCtrl = 0x31f
// // [phyinit_print_dat] mb_DDR3U_1D[0].HdtCtrl = 0xc8
// // [phyinit_print_dat] mb_DDR3U_1D[0].PhyConfigOverride = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].DFIMRLMargin = 0x1
// // [phyinit_print_dat] mb_DDR3U_1D[0].MR0 = 0xd70
// // [phyinit_print_dat] mb_DDR3U_1D[0].MR1 = 0x44
// // [phyinit_print_dat] mb_DDR3U_1D[0].MR2 = 0x18
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl0 = 0x21
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl1 = 0x12
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl2 = 0x84
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl3 = 0x48
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl4 = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl5 = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl6 = 0x0
// // [phyinit_print_dat] mb_DDR3U_1D[0].AcsmOdtCtrl7 = 0x0


// //##############################################################
// //
// // Step (A) : Bring up VDD, VDDQ, and VAA 
// //
// // See PhyInit App Note for detailed description and function usage
// //
// //############################################################## 


//zhouwei dwc_ddrphy_phyinit_userCustom_A_bringupPower ();

// [dwc_ddrphy_phyinit_userCustom_A_bringupPower] End of dwc_ddrphy_phyinit_userCustom_A_bringupPower()
// 
// 
// //##############################################################
// //
// // Step (B) Start Clocks and Reset the PHY 
// //
// // See PhyInit App Note for detailed description and function usage
// //
// //##############################################################
// 
// 
//zhouwei dwc_ddrphy_phyinit_userCustom_B_startClockResetPhy ();

// // [dwc_ddrphy_phyinit_userCustom_B_startClockResetPhy] End of dwc_ddrphy_phyinit_userCustom_B_startClockResetPhy()
// 

// //##############################################################
// //
// // Step (C) Initialize PHY Configuration 
// //
// // Load the required PHY configuration registers for the appropriate mode and memory configuration
// //
// //##############################################################
// 

// // [phyinit_C_initPhyConfig] Start of dwc_ddrphy_phyinit_C_initPhyConfig()
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming TxSlewRate::TxPreDrvMode to 0x3
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming TxSlewRate::TxPreP to 0xf
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming TxSlewRate::TxPreN to 0xf
// // [phyinit_C_initPhyConfig] ### NOTE ### Optimal setting for TxSlewRate::TxPreP and TxSlewRate::TxPreP are technology specific.
// // [phyinit_C_initPhyConfig] ### NOTE ### Please consult the "Output Slew Rate" section of HSpice Model App Note in specific technology for recommended settings

dwc_ddrphy_apb_wr(0x1005f,0x3ff); // DWC_DDRPHYA_DBYTE0_TxSlewRate_b0_p0
dwc_ddrphy_apb_wr(0x1015f,0x3ff); // DWC_DDRPHYA_DBYTE0_TxSlewRate_b1_p0
dwc_ddrphy_apb_wr(0x1105f,0x3ff); // DWC_DDRPHYA_DBYTE1_TxSlewRate_b0_p0
dwc_ddrphy_apb_wr(0x1115f,0x3ff); // DWC_DDRPHYA_DBYTE1_TxSlewRate_b1_p0
dwc_ddrphy_apb_wr(0x1205f,0x3ff); // DWC_DDRPHYA_DBYTE2_TxSlewRate_b0_p0
dwc_ddrphy_apb_wr(0x1215f,0x3ff); // DWC_DDRPHYA_DBYTE2_TxSlewRate_b1_p0
dwc_ddrphy_apb_wr(0x1305f,0x3ff); // DWC_DDRPHYA_DBYTE3_TxSlewRate_b0_p0
dwc_ddrphy_apb_wr(0x1315f,0x3ff); // DWC_DDRPHYA_DBYTE3_TxSlewRate_b1_p0
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreDrvMode to 0x3, ANIB=0
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreP to 0xf, ANIB=0
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreN to 0xf, ANIB=0
// // [phyinit_C_initPhyConfig] ### NOTE ### Optimal setting for ATxSlewRate::ATxPreP and ATxSlewRate::ATxPreP are technology specific.
// // [phyinit_C_initPhyConfig] ### NOTE ### Please consult the "Output Slew Rate" section of HSpice Model App Note in specific technology for recommended settings

dwc_ddrphy_apb_wr(0x55,0x3ff); // DWC_DDRPHYA_ANIB0_ATxSlewRate
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreDrvMode to 0x3, ANIB=1
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreP to 0xf, ANIB=1
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreN to 0xf, ANIB=1
// // [phyinit_C_initPhyConfig] ### NOTE ### Optimal setting for ATxSlewRate::ATxPreP and ATxSlewRate::ATxPreP are technology specific.
// // [phyinit_C_initPhyConfig] ### NOTE ### Please consult the "Output Slew Rate" section of HSpice Model App Note in specific technology for recommended settings

dwc_ddrphy_apb_wr(0x1055,0x3ff); // DWC_DDRPHYA_ANIB1_ATxSlewRate
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreDrvMode to 0x3, ANIB=2
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreP to 0xf, ANIB=2
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreN to 0xf, ANIB=2
// // [phyinit_C_initPhyConfig] ### NOTE ### Optimal setting for ATxSlewRate::ATxPreP and ATxSlewRate::ATxPreP are technology specific.
// // [phyinit_C_initPhyConfig] ### NOTE ### Please consult the "Output Slew Rate" section of HSpice Model App Note in specific technology for recommended settings

dwc_ddrphy_apb_wr(0x2055,0x3ff); // DWC_DDRPHYA_ANIB2_ATxSlewRate
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreDrvMode to 0x3, ANIB=3
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreP to 0xf, ANIB=3
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreN to 0xf, ANIB=3
// // [phyinit_C_initPhyConfig] ### NOTE ### Optimal setting for ATxSlewRate::ATxPreP and ATxSlewRate::ATxPreP are technology specific.
// // [phyinit_C_initPhyConfig] ### NOTE ### Please consult the "Output Slew Rate" section of HSpice Model App Note in specific technology for recommended settings

dwc_ddrphy_apb_wr(0x3055,0x3ff); // DWC_DDRPHYA_ANIB3_ATxSlewRate
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreDrvMode to 0x0, ANIB=4
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreP to 0xf, ANIB=4
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreN to 0xf, ANIB=4
// // [phyinit_C_initPhyConfig] ### NOTE ### Optimal setting for ATxSlewRate::ATxPreP and ATxSlewRate::ATxPreP are technology specific.
// // [phyinit_C_initPhyConfig] ### NOTE ### Please consult the "Output Slew Rate" section of HSpice Model App Note in specific technology for recommended settings

dwc_ddrphy_apb_wr(0x4055,0xff); // DWC_DDRPHYA_ANIB4_ATxSlewRate
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreDrvMode to 0x0, ANIB=5
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreP to 0xf, ANIB=5
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreN to 0xf, ANIB=5
// // [phyinit_C_initPhyConfig] ### NOTE ### Optimal setting for ATxSlewRate::ATxPreP and ATxSlewRate::ATxPreP are technology specific.
// // [phyinit_C_initPhyConfig] ### NOTE ### Please consult the "Output Slew Rate" section of HSpice Model App Note in specific technology for recommended settings

dwc_ddrphy_apb_wr(0x5055,0xff); // DWC_DDRPHYA_ANIB5_ATxSlewRate
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreDrvMode to 0x3, ANIB=6
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreP to 0xf, ANIB=6
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreN to 0xf, ANIB=6
// // [phyinit_C_initPhyConfig] ### NOTE ### Optimal setting for ATxSlewRate::ATxPreP and ATxSlewRate::ATxPreP are technology specific.
// // [phyinit_C_initPhyConfig] ### NOTE ### Please consult the "Output Slew Rate" section of HSpice Model App Note in specific technology for recommended settings

dwc_ddrphy_apb_wr(0x6055,0x3ff); // DWC_DDRPHYA_ANIB6_ATxSlewRate
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreDrvMode to 0x3, ANIB=7
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreP to 0xf, ANIB=7
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreN to 0xf, ANIB=7
// // [phyinit_C_initPhyConfig] ### NOTE ### Optimal setting for ATxSlewRate::ATxPreP and ATxSlewRate::ATxPreP are technology specific.
// // [phyinit_C_initPhyConfig] ### NOTE ### Please consult the "Output Slew Rate" section of HSpice Model App Note in specific technology for recommended settings

dwc_ddrphy_apb_wr(0x7055,0x3ff); // DWC_DDRPHYA_ANIB7_ATxSlewRate
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreDrvMode to 0x3, ANIB=8
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreP to 0xf, ANIB=8
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreN to 0xf, ANIB=8
// // [phyinit_C_initPhyConfig] ### NOTE ### Optimal setting for ATxSlewRate::ATxPreP and ATxSlewRate::ATxPreP are technology specific.
// // [phyinit_C_initPhyConfig] ### NOTE ### Please consult the "Output Slew Rate" section of HSpice Model App Note in specific technology for recommended settings

dwc_ddrphy_apb_wr(0x8055,0x3ff); // DWC_DDRPHYA_ANIB8_ATxSlewRate
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreDrvMode to 0x3, ANIB=9
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreP to 0xf, ANIB=9
// // [phyinit_C_initPhyConfig] Programming ATxSlewRate::ATxPreN to 0xf, ANIB=9
// // [phyinit_C_initPhyConfig] ### NOTE ### Optimal setting for ATxSlewRate::ATxPreP and ATxSlewRate::ATxPreP are technology specific.
// // [phyinit_C_initPhyConfig] ### NOTE ### Please consult the "Output Slew Rate" section of HSpice Model App Note in specific technology for recommended settings

dwc_ddrphy_apb_wr(0x9055,0x3ff); // DWC_DDRPHYA_ANIB9_ATxSlewRate
dwc_ddrphy_apb_wr(0x200c5,0xb); // DWC_DDRPHYA_MASTER0_PllCtrl2_p0
// // [phyinit_C_initPhyConfig] Pstate=0,  Memclk=800MHz, Programming PllCtrl2 to b based on DfiClk frequency = 400.
// 
// //##############################################################
// //
// // Program ARdPtrInitVal based on Frequency and PLL Bypass inputs
// // The values programmed here assume ideal properties of DfiClk
// // and Pclk including:
// // - DfiClk skew
// // - DfiClk jitter
// // - DfiClk PVT variations
// // - Pclk skew
// // - Pclk jitter
// //
// // PLL Bypassed mode:
// //     For MemClk frequency > 933MHz, the valid range of ARdPtrInitVal_p0[3:0] is: 2-6
// //     For MemClk frequency < 933MHz, the valid range of ARdPtrInitVal_p0[3:0] is: 1-6
// //
// // PLL Enabled mode:
// //     For MemClk frequency > 933MHz, the valid range of ARdPtrInitVal_p0[3:0] is: 1-6
// //     For MemClk frequency < 933MHz, the valid range of ARdPtrInitVal_p0[3:0] is: 0-6
// //
// //##############################################################
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming ARdPtrInitVal to 0x1
dwc_ddrphy_apb_wr(0x2002e,0x1); // DWC_DDRPHYA_MASTER0_ARdPtrInitVal_p0
// 
// //##############################################################
// // TxPreDrvMode[2] = 0
// //##############################################################
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming DqsPreambleControl::TwoTckRxDqsPre to 0x0
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming DqsPreambleControl::TwoTckTxDqsPre to 0x0
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming DqsPreambleControl::PositionDfeInit to 0x2
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming DqsPreambleControl::LP4TglTwoTckTxDqsPre to 0x0
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming DqsPreambleControl::LP4PostambleExt to 0x0
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming DqsPreambleControl::LP4SttcPreBridgeRxEn to 0x0
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming DqsPreambleControl to 0x8
dwc_ddrphy_apb_wr(0x20024,0x8); // DWC_DDRPHYA_MASTER0_DqsPreambleControl_p0
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming DbyteDllModeCntrl to 0x0
dwc_ddrphy_apb_wr(0x2003a,0x0); // DWC_DDRPHYA_MASTER0_DbyteDllModeCntrl
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming ProcOdtTimeCtl to 0xa
dwc_ddrphy_apb_wr(0x20056,0xa); // DWC_DDRPHYA_MASTER0_ProcOdtTimeCtl_p0
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming TxOdtDrvStren::ODTStrenP to 0x1a
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming TxOdtDrvStren::ODTStrenN to 0x1a
dwc_ddrphy_apb_wr(0x1004d,0x69a); // DWC_DDRPHYA_DBYTE0_TxOdtDrvStren_b0_p0
dwc_ddrphy_apb_wr(0x1014d,0x69a); // DWC_DDRPHYA_DBYTE0_TxOdtDrvStren_b1_p0
dwc_ddrphy_apb_wr(0x1104d,0x69a); // DWC_DDRPHYA_DBYTE1_TxOdtDrvStren_b0_p0
dwc_ddrphy_apb_wr(0x1114d,0x69a); // DWC_DDRPHYA_DBYTE1_TxOdtDrvStren_b1_p0
dwc_ddrphy_apb_wr(0x1204d,0x69a); // DWC_DDRPHYA_DBYTE2_TxOdtDrvStren_b0_p0
dwc_ddrphy_apb_wr(0x1214d,0x69a); // DWC_DDRPHYA_DBYTE2_TxOdtDrvStren_b1_p0
dwc_ddrphy_apb_wr(0x1304d,0x69a); // DWC_DDRPHYA_DBYTE3_TxOdtDrvStren_b0_p0
dwc_ddrphy_apb_wr(0x1314d,0x69a); // DWC_DDRPHYA_DBYTE3_TxOdtDrvStren_b1_p0
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming TxImpedanceCtrl1::DrvStrenFSDqP to 0x38
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming TxImpedanceCtrl1::DrvStrenFSDqN to 0x38
dwc_ddrphy_apb_wr(0x10049,0xe38); // DWC_DDRPHYA_DBYTE0_TxImpedanceCtrl1_b0_p0
dwc_ddrphy_apb_wr(0x10149,0xe38); // DWC_DDRPHYA_DBYTE0_TxImpedanceCtrl1_b1_p0
dwc_ddrphy_apb_wr(0x11049,0xe38); // DWC_DDRPHYA_DBYTE1_TxImpedanceCtrl1_b0_p0
dwc_ddrphy_apb_wr(0x11149,0xe38); // DWC_DDRPHYA_DBYTE1_TxImpedanceCtrl1_b1_p0
dwc_ddrphy_apb_wr(0x12049,0xe38); // DWC_DDRPHYA_DBYTE2_TxImpedanceCtrl1_b0_p0
dwc_ddrphy_apb_wr(0x12149,0xe38); // DWC_DDRPHYA_DBYTE2_TxImpedanceCtrl1_b1_p0
dwc_ddrphy_apb_wr(0x13049,0xe38); // DWC_DDRPHYA_DBYTE3_TxImpedanceCtrl1_b0_p0
dwc_ddrphy_apb_wr(0x13149,0xe38); // DWC_DDRPHYA_DBYTE3_TxImpedanceCtrl1_b1_p0
// // [phyinit_C_initPhyConfig] Programming ATxImpedance::ADrvStrenP to 0x1f
// // [phyinit_C_initPhyConfig] Programming ATxImpedance::ADrvStrenN to 0x1f
dwc_ddrphy_apb_wr(0x43,0x3ff); // DWC_DDRPHYA_ANIB0_ATxImpedance
dwc_ddrphy_apb_wr(0x1043,0x3ff); // DWC_DDRPHYA_ANIB1_ATxImpedance
dwc_ddrphy_apb_wr(0x2043,0x3ff); // DWC_DDRPHYA_ANIB2_ATxImpedance
dwc_ddrphy_apb_wr(0x3043,0x3ff); // DWC_DDRPHYA_ANIB3_ATxImpedance
dwc_ddrphy_apb_wr(0x4043,0x3ff); // DWC_DDRPHYA_ANIB4_ATxImpedance
dwc_ddrphy_apb_wr(0x5043,0x3ff); // DWC_DDRPHYA_ANIB5_ATxImpedance
dwc_ddrphy_apb_wr(0x6043,0x3ff); // DWC_DDRPHYA_ANIB6_ATxImpedance
dwc_ddrphy_apb_wr(0x7043,0x3ff); // DWC_DDRPHYA_ANIB7_ATxImpedance
dwc_ddrphy_apb_wr(0x8043,0x3ff); // DWC_DDRPHYA_ANIB8_ATxImpedance
dwc_ddrphy_apb_wr(0x9043,0x3ff); // DWC_DDRPHYA_ANIB9_ATxImpedance
// // [phyinit_C_initPhyConfig] Programming DfiMode to 0x5
dwc_ddrphy_apb_wr(0x20018,0x5); // DWC_DDRPHYA_MASTER0_DfiMode
// // [phyinit_C_initPhyConfig] Programming DfiCAMode to 0x0
dwc_ddrphy_apb_wr(0x20075,0x0); // DWC_DDRPHYA_MASTER0_DfiCAMode
// // [phyinit_C_initPhyConfig] Programming CalDrvStr0::CalDrvStrPd50 to 0x0
// // [phyinit_C_initPhyConfig] Programming CalDrvStr0::CalDrvStrPu50 to 0x0
dwc_ddrphy_apb_wr(0x20050,0x0); // DWC_DDRPHYA_MASTER0_CalDrvStr0
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming CalUclkInfo::CalUClkTicksPer1uS to 0x190
dwc_ddrphy_apb_wr(0x20008,0x190); // DWC_DDRPHYA_MASTER0_CalUclkInfo_p0
// // [phyinit_C_initPhyConfig] Programming CalRate::CalInterval to 0x9
// // [phyinit_C_initPhyConfig] Programming CalRate::CalOnce to 0x0
dwc_ddrphy_apb_wr(0x20088,0x9); // DWC_DDRPHYA_MASTER0_CalRate
// // [phyinit_C_initPhyConfig] Pstate=0, Programming VrefInGlobal::GlobalVrefInSel to 0x0
// // [phyinit_C_initPhyConfig] Pstate=0, Programming VrefInGlobal::GlobalVrefInDAC to 0x1f
// // [phyinit_C_initPhyConfig] Pstate=0, Programming VrefInGlobal to 0xf8
dwc_ddrphy_apb_wr(0x200b2,0xf8); // DWC_DDRPHYA_MASTER0_VrefInGlobal_p0
// // [phyinit_C_initPhyConfig] Pstate=0, Programming DqDqsRcvCntrl::MajorModeDbyte to 0x0
// // [phyinit_C_initPhyConfig] Pstate=0, Programming DqDqsRcvCntrl to 0x581
dwc_ddrphy_apb_wr(0x10043,0x581); // DWC_DDRPHYA_DBYTE0_DqDqsRcvCntrl_b0_p0
dwc_ddrphy_apb_wr(0x10143,0x581); // DWC_DDRPHYA_DBYTE0_DqDqsRcvCntrl_b1_p0
dwc_ddrphy_apb_wr(0x11043,0x581); // DWC_DDRPHYA_DBYTE1_DqDqsRcvCntrl_b0_p0
dwc_ddrphy_apb_wr(0x11143,0x581); // DWC_DDRPHYA_DBYTE1_DqDqsRcvCntrl_b1_p0
dwc_ddrphy_apb_wr(0x12043,0x581); // DWC_DDRPHYA_DBYTE2_DqDqsRcvCntrl_b0_p0
dwc_ddrphy_apb_wr(0x12143,0x581); // DWC_DDRPHYA_DBYTE2_DqDqsRcvCntrl_b1_p0
dwc_ddrphy_apb_wr(0x13043,0x581); // DWC_DDRPHYA_DBYTE3_DqDqsRcvCntrl_b0_p0
dwc_ddrphy_apb_wr(0x13143,0x581); // DWC_DDRPHYA_DBYTE3_DqDqsRcvCntrl_b1_p0
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming DfiFreqRatio_p0 to 0x1
dwc_ddrphy_apb_wr(0x200fa,0x1); // DWC_DDRPHYA_MASTER0_DfiFreqRatio_p0
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming TristateModeCA::DisDynAdrTri_p0 to 0x1
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming TristateModeCA::DDR2TMode_p0 to 0x0
dwc_ddrphy_apb_wr(0x20019,0x5); // DWC_DDRPHYA_MASTER0_TristateModeCA_p0
// // [phyinit_C_initPhyConfig] Programming DfiFreqXlat*
dwc_ddrphy_apb_wr(0x200f0,0x5555); // DWC_DDRPHYA_MASTER0_DfiFreqXlat0
dwc_ddrphy_apb_wr(0x200f1,0x5555); // DWC_DDRPHYA_MASTER0_DfiFreqXlat1
dwc_ddrphy_apb_wr(0x200f2,0x5555); // DWC_DDRPHYA_MASTER0_DfiFreqXlat2
dwc_ddrphy_apb_wr(0x200f3,0x5555); // DWC_DDRPHYA_MASTER0_DfiFreqXlat3
dwc_ddrphy_apb_wr(0x200f4,0x5555); // DWC_DDRPHYA_MASTER0_DfiFreqXlat4
dwc_ddrphy_apb_wr(0x200f5,0x5555); // DWC_DDRPHYA_MASTER0_DfiFreqXlat5
dwc_ddrphy_apb_wr(0x200f6,0x5555); // DWC_DDRPHYA_MASTER0_DfiFreqXlat6
dwc_ddrphy_apb_wr(0x200f7,0xf000); // DWC_DDRPHYA_MASTER0_DfiFreqXlat7
// // [phyinit_C_initPhyConfig] Pstate=0,  Memclk=800MHz, Programming Seq0BDLY0 to 0x33
dwc_ddrphy_apb_wr(0x2000b,0x33); // DWC_DDRPHYA_MASTER0_Seq0BDLY0_p0
// // [phyinit_C_initPhyConfig] Pstate=0,  Memclk=800MHz, Programming Seq0BDLY1 to 0x65
dwc_ddrphy_apb_wr(0x2000c,0x65); // DWC_DDRPHYA_MASTER0_Seq0BDLY1_p0
// // [phyinit_C_initPhyConfig] Pstate=0,  Memclk=800MHz, Programming Seq0BDLY2 to 0x3e9
dwc_ddrphy_apb_wr(0x2000d,0x3e9); // DWC_DDRPHYA_MASTER0_Seq0BDLY2_p0
// // [phyinit_C_initPhyConfig] Pstate=0,  Memclk=800MHz, Programming Seq0BDLY3 to 0x2c
dwc_ddrphy_apb_wr(0x2000e,0x2c); // DWC_DDRPHYA_MASTER0_Seq0BDLY3_p0
// // [phyinit_C_initPhyConfig] Programming MasterX4Config::X4TG to 0x0
dwc_ddrphy_apb_wr(0x20025,0x0); // DWC_DDRPHYA_MASTER0_MasterX4Config
// // [phyinit_C_initPhyConfig] Pstate=0, Memclk=800MHz, Programming DMIPinPresent::RdDbiEnabled to 0x0
dwc_ddrphy_apb_wr(0x2002d,0x0); // DWC_DDRPHYA_MASTER0_DMIPinPresent_p0
// // [phyinit_C_initPhyConfig] End of dwc_ddrphy_phyinit_C_initPhyConfig()
// 
if(!after_retention){
// 
// //##############################################################
// //
// // dwc_ddrphy_phyihunit_userCustom_customPreTrain is a user-editable function.
// //
// // See PhyInit App Note for detailed description and function usage
// //
// //##############################################################
// 
// // [phyinit_userCustom_customPreTrain] Start of dwc_ddrphy_phyinit_userCustom_customPreTrain()
// // [phyinit_userCustom_customPreTrain] End of dwc_ddrphy_phyinit_userCustom_customPreTrain()
// // [dwc_ddrphy_phyinit_D_loadIMEM, 1D] Start of dwc_ddrphy_phyinit_D_loadIMEM (Train2D=0)
// 
// 
// //##############################################################
// //
// // (D) Load the 1D IMEM image
// // 
// // This function loads the training firmware IMEM image into the SRAM.
// // See PhyInit App Note for detailed description and function usage
// // 
// //##############################################################
// 
// 
// // [dwc_ddrphy_phyinit_D_loadIMEM, 1D] Programming MemResetL to 0x2
dwc_ddrphy_apb_wr(0x20060,0x2); // DWC_DDRPHYA_MASTER0_MemResetL
// [dwc_ddrphy_phyinit_storeIncvFile] Reading input file: /ux/m850D/weizhou/from_snps/snps_lpddr4mv2_fw_phyinit_a_2017_05_ctb_0_30b/firmware_lpddr4_mphy_v2_vA-2017.05/firmware/Latest/ddr3/ddr3_pmu_train_imem.incv

// // 1.	Enable access to the internal CSRs by setting the MicroContMuxSel CSR to 0.
// //       This allows the memory controller unrestricted access to the configuration CSRs. 
dwc_ddrphy_apb_wr(0xd0000,0x0); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel
// // [dwc_ddrphy_phyinit_WriteOutMem] STARTING. offset 0x50000 size 0x4000
//dwc_ddrphy_apb_wr(0x50000,0xb0);
//...
//dwc_ddrphy_apb_wr(0x53fff,0x0);
// // [dwc_ddrphy_phyinit_WriteOutMem] DONE.  Index 0x4000
// // 2.	Isolate the APB access from the internal CSRs by setting the MicroContMuxSel CSR to 1. 
// //      This allows the firmware unrestricted access to the configuration CSRs. 
dwc_ddrphy_apb_wr(0xd0000,0x1); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel
// // [dwc_ddrphy_phyinit_D_loadIMEM, 1D] End of dwc_ddrphy_phyinit_D_loadIMEM()
// 
// 
// //##############################################################
// //
// // Step (E) Set the PHY input clocks to the desired frequency for pstate 0 
// // 
// // See PhyInit App Note for detailed description and function usage
// // 
// //##############################################################
// 
//zhouwei dwc_ddrphy_phyinit_userCustom_E_setDfiClk (0);

// 
// // [dwc_ddrphy_phyinit_userCustom_E_setDfiClk] End of dwc_ddrphy_phyinit_userCustom_E_setDfiClk()
// // [phyinit_F_loadDMEM, 1D] Start of dwc_ddrphy_phyinit_F_loadDMEM (pstate=0, Train2D=0)
// 
// 
// //##############################################################
// //
// // 4.3.5(F) Load the 1D DMEM image and write the 1D Message Block parameters for the training firmware 
// // 
// // The procedure is as follows: 
// // 
// //##############################################################
// 
// 
// 
// // 1.    Load the firmware DMEM segment to initialize the data structures.
// 
// // 2.    Write the Firmware Message Block with the required contents detailing the training parameters.
// 
// [dwc_ddrphy_phyinit_storeIncvFile] Reading input file: /ux/m850D/weizhou/from_snps/snps_lpddr4mv2_fw_phyinit_a_2017_05_ctb_0_30b/firmware_lpddr4_mphy_v2_vA-2017.05/firmware/Latest/ddr3/ddr3_pmu_train_dmem.incv

// // 1.	Enable access to the internal CSRs by setting the MicroContMuxSel CSR to 0.
// //       This allows the memory controller unrestricted access to the configuration CSRs. 
dwc_ddrphy_apb_wr(0xd0000,0x0); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel
// // [dwc_ddrphy_phyinit_WriteOutMem] STARTING. offset 0x54000 size 0x364
#ifdef RUN_ON_SILICON
dwc_ddrphy_apb_wr(0x54000,0x0);
#else
dwc_ddrphy_apb_wr(0x54000,0x600);
#endif
dwc_ddrphy_apb_wr(0x54001,0x0);
dwc_ddrphy_apb_wr(0x54002,0x0);
dwc_ddrphy_apb_wr(0x54003,0x640);
dwc_ddrphy_apb_wr(0x54004,0x2);
dwc_ddrphy_apb_wr(0x54005,0x283c);
dwc_ddrphy_apb_wr(0x54006,0x13b);
dwc_ddrphy_apb_wr(0x54007,0x2000);

#ifdef DDR_ONE_RANK
dwc_ddrphy_apb_wr(0x54008,0x301);//rank0
#else
dwc_ddrphy_apb_wr(0x54008,0x303);
#endif

dwc_ddrphy_apb_wr(0x54009,0x0);
dwc_ddrphy_apb_wr(0x5400a,0x0);
#ifdef RUN_ON_SILICON
dwc_ddrphy_apb_wr(0x5400b,0x31f);
#else
dwc_ddrphy_apb_wr(0x5400b,0x1);
#endif
dwc_ddrphy_apb_wr(0x5400c,0xc8);
//dwc_ddrphy_apb_wr(0x5400d,0x0);
//dwc_ddrphy_apb_wr(0x5400e,0x0);
//dwc_ddrphy_apb_wr(0x5400f,0x0);
//dwc_ddrphy_apb_wr(0x54010,0x0);
//dwc_ddrphy_apb_wr(0x54011,0x0);
dwc_ddrphy_apb_wr(0x54012,0x1);
//dwc_ddrphy_apb_wr(0x54013,0x0);
//dwc_ddrphy_apb_wr(0x54014,0x0);
//dwc_ddrphy_apb_wr(0x54015,0x0);
//dwc_ddrphy_apb_wr(0x54016,0x0);
//dwc_ddrphy_apb_wr(0x54017,0x0);
//dwc_ddrphy_apb_wr(0x54018,0x0);
//dwc_ddrphy_apb_wr(0x54019,0x0);
//dwc_ddrphy_apb_wr(0x5401a,0x0);
//dwc_ddrphy_apb_wr(0x5401b,0x0);
//dwc_ddrphy_apb_wr(0x5401c,0x0);
//dwc_ddrphy_apb_wr(0x5401d,0x0);
//dwc_ddrphy_apb_wr(0x5401e,0x0);
//dwc_ddrphy_apb_wr(0x5401f,0x0);
//dwc_ddrphy_apb_wr(0x54020,0x0);
//dwc_ddrphy_apb_wr(0x54021,0x0);
//dwc_ddrphy_apb_wr(0x54022,0x0);
//dwc_ddrphy_apb_wr(0x54023,0x0);
//dwc_ddrphy_apb_wr(0x54024,0x0);
//dwc_ddrphy_apb_wr(0x54025,0x0);
//dwc_ddrphy_apb_wr(0x54026,0x0);
//dwc_ddrphy_apb_wr(0x54027,0x0);
//dwc_ddrphy_apb_wr(0x54028,0x0);
//dwc_ddrphy_apb_wr(0x54029,0x0);
//dwc_ddrphy_apb_wr(0x5402a,0x0);
//dwc_ddrphy_apb_wr(0x5402b,0x0);
//dwc_ddrphy_apb_wr(0x5402c,0x0);
//dwc_ddrphy_apb_wr(0x5402d,0x0);
//dwc_ddrphy_apb_wr(0x5402e,0x0);
dwc_ddrphy_apb_wr(0x5402f,0xd70);
dwc_ddrphy_apb_wr(0x54030,0x04);
dwc_ddrphy_apb_wr(0x54031,0x18);
//dwc_ddrphy_apb_wr(0x54032,0x0);
//dwc_ddrphy_apb_wr(0x54033,0x0);
//dwc_ddrphy_apb_wr(0x54034,0x0);
//dwc_ddrphy_apb_wr(0x54035,0x0);
//dwc_ddrphy_apb_wr(0x54036,0x0);
//dwc_ddrphy_apb_wr(0x54037,0x0);
//dwc_ddrphy_apb_wr(0x54038,0x0);
//dwc_ddrphy_apb_wr(0x54039,0x0);
dwc_ddrphy_apb_wr(0x5403a,0x1221);
dwc_ddrphy_apb_wr(0x5403b,0x4884);
//dwc_ddrphy_apb_wr(0x5403c,0x0);
//dwc_ddrphy_apb_wr(0x5403d,0x0);
//dwc_ddrphy_apb_wr(0x5403e,0x0);
//dwc_ddrphy_apb_wr(0x5403f,0x0);
//dwc_ddrphy_apb_wr(0x54040,0x0);
//dwc_ddrphy_apb_wr(0x54041,0x0);
//dwc_ddrphy_apb_wr(0x54042,0x0);
//dwc_ddrphy_apb_wr(0x54043,0x0);
//dwc_ddrphy_apb_wr(0x54044,0x0);
//dwc_ddrphy_apb_wr(0x54045,0x0);
//dwc_ddrphy_apb_wr(0x54046,0x0);
//dwc_ddrphy_apb_wr(0x54047,0x0);
//dwc_ddrphy_apb_wr(0x54048,0x0);
//dwc_ddrphy_apb_wr(0x54049,0x0);
//dwc_ddrphy_apb_wr(0x5404a,0x0);
//dwc_ddrphy_apb_wr(0x5404b,0x0);
//dwc_ddrphy_apb_wr(0x5404c,0x0);
//dwc_ddrphy_apb_wr(0x5404d,0x0);
//dwc_ddrphy_apb_wr(0x5404e,0x0);
//dwc_ddrphy_apb_wr(0x5404f,0x0);
//dwc_ddrphy_apb_wr(0x54050,0x0);
//dwc_ddrphy_apb_wr(0x54051,0x0);
// // [dwc_ddrphy_phyinit_WriteOutMem] DONE.  Index 0x364
// // 2.	Isolate the APB access from the internal CSRs by setting the MicroContMuxSel CSR to 1. 
// //      This allows the firmware unrestricted access to the configuration CSRs. 
dwc_ddrphy_apb_wr(0xd0000,0x1); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel
// // [phyinit_F_loadDMEM, 1D] End of dwc_ddrphy_phyinit_F_loadDMEM()
// 
// 
// //##############################################################
// //
// // 4.3.7(G) Execute the Training Firmware 
// // 
// // The training firmware is executed with the following procedure: 
// //
// //##############################################################
// 
// 
// // 1.  Reset the firmware microcontroller by writing the MicroReset CSR to set the StallToMicro and 
// //     ResetToMicro fields to 1 (all other fields should be zero). 
// //     Then rewrite the CSR so that only the StallToMicro remains set (all other fields should be zero). 
dwc_ddrphy_apb_wr(0xd0000,0x1); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel
dwc_ddrphy_apb_wr(0xd0099,0x9); // DWC_DDRPHYA_APBONLY0_MicroReset
dwc_ddrphy_apb_wr(0xd0099,0x1); // DWC_DDRPHYA_APBONLY0_MicroReset
// 
// // 2. Begin execution of the training firmware by setting the MicroReset CSR to 4'b0000. 
dwc_ddrphy_apb_wr(0xd0099,0x0); // DWC_DDRPHYA_APBONLY0_MicroReset
// 
// // 3.   Wait for the training firmware to complete by following the procedure in "uCtrl Initialization and Mailbox Messaging" 
// // 4.3.7  3.   Wait for the training firmware to complete.  Implement timeout fucntion or follow the procedure in "3.4 Running the firmware" of the Training Firmware Application Note to poll the Mailbox message.
dwc_ddrphy_phyinit_userCustom_G_waitFwDone ();

// // [dwc_ddrphy_phyinit_userCustom_G_waitFwDone] End of dwc_ddrphy_phyinit_userCustom_G_waitFwDone()
// // 4.   Halt the microcontroller." 
dwc_ddrphy_apb_wr(0xd0099,0x1); // DWC_DDRPHYA_APBONLY0_MicroReset
// // [dwc_ddrphy_phyinit_G_execFW] End of dwc_ddrphy_phyinit_G_execFW ()
// 
// 
// //##############################################################
// //
// // 4.3.8(H) Read the Message Block results
// // 
// // The procedure is as follows:
// // 
// //##############################################################
// 
// 
// // 1.	Enable access to the internal CSRs by setting the MicroContMuxSel CSR to 0.
dwc_ddrphy_apb_wr(0xd0000,0x0); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel
// 
// 2. Read the Firmware Message Block to obtain the results from the training.
// This can be accomplished by issuing APB read commands to the DMEM addresses.
// Example:
// if (Train2D)
// { 
//   _read_2d_message_block_outputs_
// }
// else
// {
//   _read_1d_message_block_outputs_
// }
// This can be accomplished by issuing APB read commands to the DMEM addresses.
//zhouwei dwc_ddrphy_phyinit_userCustom_H_readMsgBlock (0);

// [dwc_ddrphy_phyinit_userCustom_H_readMsgBlock] End of dwc_ddrphy_phyinit_userCustom_H_readMsgBlock ()
// // 3.	Isolate the APB access from the internal CSRs by setting the MicroContMuxSel CSR to 1. 
dwc_ddrphy_apb_wr(0xd0000,0x1); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel
// // 4.	If training is required at another frequency, repeat the operations starting at step (E). 
// // [dwc_ddrphy_phyinit_H_readMsgBlock] End of dwc_ddrphy_phyinit_H_readMsgBlock()
// // [phyinit_I_loadPIEImage] Start of dwc_ddrphy_phyinit_I_loadPIEImage()
// 
// printf("\n\n\n1D training done!!!!\n\n\n");

}
else {
//restore_1d2d_trained_csr(SAVE_DDRPHY_TRAIN_ADDR);
}
// 
// //##############################################################
// //
// // 4.3.9(I) Load PHY Init Engine Image 
// // 
// // Load the PHY Initialization Engine memory with the provided initialization sequence.
// // 
// // <Note: For LPDDR3/LPDDR4, this sequence will include the necessary retraining code.>
// // 
// //##############################################################
// 
// 
// // Enable access to the internal CSRs by setting the MicroContMuxSel CSR to 0. 
// // This allows the memory controller unrestricted access to the configuration CSRs. 
dwc_ddrphy_apb_wr(0xd0000,0x0); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel
// // [phyinit_I_loadPIEImage] Programming PIE Production Code
dwc_ddrphy_apb_wr(0x90000,0x10); // DWC_DDRPHYA_INITENG0_PreSequenceReg0b0s0
dwc_ddrphy_apb_wr(0x90001,0x400); // DWC_DDRPHYA_INITENG0_PreSequenceReg0b0s1
dwc_ddrphy_apb_wr(0x90002,0x10e); // DWC_DDRPHYA_INITENG0_PreSequenceReg0b0s2
dwc_ddrphy_apb_wr(0x90003,0x0); // DWC_DDRPHYA_INITENG0_PreSequenceReg0b1s0
dwc_ddrphy_apb_wr(0x90004,0x0); // DWC_DDRPHYA_INITENG0_PreSequenceReg0b1s1
dwc_ddrphy_apb_wr(0x90005,0x8); // DWC_DDRPHYA_INITENG0_PreSequenceReg0b1s2
dwc_ddrphy_apb_wr(0x90029,0xb); // DWC_DDRPHYA_INITENG0_SequenceReg0b0s0
dwc_ddrphy_apb_wr(0x9002a,0x480); // DWC_DDRPHYA_INITENG0_SequenceReg0b0s1
dwc_ddrphy_apb_wr(0x9002b,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b0s2
dwc_ddrphy_apb_wr(0x9002c,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b1s0
dwc_ddrphy_apb_wr(0x9002d,0x448); // DWC_DDRPHYA_INITENG0_SequenceReg0b1s1
dwc_ddrphy_apb_wr(0x9002e,0x139); // DWC_DDRPHYA_INITENG0_SequenceReg0b1s2
dwc_ddrphy_apb_wr(0x9002f,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b2s0
dwc_ddrphy_apb_wr(0x90030,0x478); // DWC_DDRPHYA_INITENG0_SequenceReg0b2s1
dwc_ddrphy_apb_wr(0x90031,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b2s2
dwc_ddrphy_apb_wr(0x90032,0x2); // DWC_DDRPHYA_INITENG0_SequenceReg0b3s0
dwc_ddrphy_apb_wr(0x90033,0x10); // DWC_DDRPHYA_INITENG0_SequenceReg0b3s1
dwc_ddrphy_apb_wr(0x90034,0x139); // DWC_DDRPHYA_INITENG0_SequenceReg0b3s2
dwc_ddrphy_apb_wr(0x90035,0xf); // DWC_DDRPHYA_INITENG0_SequenceReg0b4s0
dwc_ddrphy_apb_wr(0x90036,0x7c0); // DWC_DDRPHYA_INITENG0_SequenceReg0b4s1
dwc_ddrphy_apb_wr(0x90037,0x139); // DWC_DDRPHYA_INITENG0_SequenceReg0b4s2
dwc_ddrphy_apb_wr(0x90038,0x44); // DWC_DDRPHYA_INITENG0_SequenceReg0b5s0
dwc_ddrphy_apb_wr(0x90039,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b5s1
dwc_ddrphy_apb_wr(0x9003a,0x159); // DWC_DDRPHYA_INITENG0_SequenceReg0b5s2
dwc_ddrphy_apb_wr(0x9003b,0x14f); // DWC_DDRPHYA_INITENG0_SequenceReg0b6s0
dwc_ddrphy_apb_wr(0x9003c,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b6s1
dwc_ddrphy_apb_wr(0x9003d,0x159); // DWC_DDRPHYA_INITENG0_SequenceReg0b6s2
dwc_ddrphy_apb_wr(0x9003e,0x47); // DWC_DDRPHYA_INITENG0_SequenceReg0b7s0
dwc_ddrphy_apb_wr(0x9003f,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b7s1
dwc_ddrphy_apb_wr(0x90040,0x149); // DWC_DDRPHYA_INITENG0_SequenceReg0b7s2
dwc_ddrphy_apb_wr(0x90041,0x4f); // DWC_DDRPHYA_INITENG0_SequenceReg0b8s0
dwc_ddrphy_apb_wr(0x90042,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b8s1
dwc_ddrphy_apb_wr(0x90043,0x179); // DWC_DDRPHYA_INITENG0_SequenceReg0b8s2
dwc_ddrphy_apb_wr(0x90044,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b9s0
dwc_ddrphy_apb_wr(0x90045,0xe0); // DWC_DDRPHYA_INITENG0_SequenceReg0b9s1
dwc_ddrphy_apb_wr(0x90046,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b9s2
dwc_ddrphy_apb_wr(0x90047,0x0); // DWC_DDRPHYA_INITENG0_SequenceReg0b10s0
dwc_ddrphy_apb_wr(0x90048,0x7c8); // DWC_DDRPHYA_INITENG0_SequenceReg0b10s1
dwc_ddrphy_apb_wr(0x90049,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b10s2
dwc_ddrphy_apb_wr(0x9004a,0x0); // DWC_DDRPHYA_INITENG0_SequenceReg0b11s0
dwc_ddrphy_apb_wr(0x9004b,0x1); // DWC_DDRPHYA_INITENG0_SequenceReg0b11s1
dwc_ddrphy_apb_wr(0x9004c,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b11s2
dwc_ddrphy_apb_wr(0x9004d,0x0); // DWC_DDRPHYA_INITENG0_SequenceReg0b12s0
dwc_ddrphy_apb_wr(0x9004e,0x45a); // DWC_DDRPHYA_INITENG0_SequenceReg0b12s1
dwc_ddrphy_apb_wr(0x9004f,0x9); // DWC_DDRPHYA_INITENG0_SequenceReg0b12s2
dwc_ddrphy_apb_wr(0x90050,0x0); // DWC_DDRPHYA_INITENG0_SequenceReg0b13s0
dwc_ddrphy_apb_wr(0x90051,0x448); // DWC_DDRPHYA_INITENG0_SequenceReg0b13s1
dwc_ddrphy_apb_wr(0x90052,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b13s2
dwc_ddrphy_apb_wr(0x90053,0x40); // DWC_DDRPHYA_INITENG0_SequenceReg0b14s0
dwc_ddrphy_apb_wr(0x90054,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b14s1
dwc_ddrphy_apb_wr(0x90055,0x179); // DWC_DDRPHYA_INITENG0_SequenceReg0b14s2
dwc_ddrphy_apb_wr(0x90056,0x1); // DWC_DDRPHYA_INITENG0_SequenceReg0b15s0
dwc_ddrphy_apb_wr(0x90057,0x618); // DWC_DDRPHYA_INITENG0_SequenceReg0b15s1
dwc_ddrphy_apb_wr(0x90058,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b15s2
dwc_ddrphy_apb_wr(0x90059,0x40c0); // DWC_DDRPHYA_INITENG0_SequenceReg0b16s0
dwc_ddrphy_apb_wr(0x9005a,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b16s1
dwc_ddrphy_apb_wr(0x9005b,0x149); // DWC_DDRPHYA_INITENG0_SequenceReg0b16s2
dwc_ddrphy_apb_wr(0x9005c,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b17s0
dwc_ddrphy_apb_wr(0x9005d,0x4); // DWC_DDRPHYA_INITENG0_SequenceReg0b17s1
dwc_ddrphy_apb_wr(0x9005e,0x48); // DWC_DDRPHYA_INITENG0_SequenceReg0b17s2
dwc_ddrphy_apb_wr(0x9005f,0x4040); // DWC_DDRPHYA_INITENG0_SequenceReg0b18s0
dwc_ddrphy_apb_wr(0x90060,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b18s1
dwc_ddrphy_apb_wr(0x90061,0x149); // DWC_DDRPHYA_INITENG0_SequenceReg0b18s2
dwc_ddrphy_apb_wr(0x90062,0x0); // DWC_DDRPHYA_INITENG0_SequenceReg0b19s0
dwc_ddrphy_apb_wr(0x90063,0x4); // DWC_DDRPHYA_INITENG0_SequenceReg0b19s1
dwc_ddrphy_apb_wr(0x90064,0x48); // DWC_DDRPHYA_INITENG0_SequenceReg0b19s2
dwc_ddrphy_apb_wr(0x90065,0x40); // DWC_DDRPHYA_INITENG0_SequenceReg0b20s0
dwc_ddrphy_apb_wr(0x90066,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b20s1
dwc_ddrphy_apb_wr(0x90067,0x149); // DWC_DDRPHYA_INITENG0_SequenceReg0b20s2
dwc_ddrphy_apb_wr(0x90068,0x10); // DWC_DDRPHYA_INITENG0_SequenceReg0b21s0
dwc_ddrphy_apb_wr(0x90069,0x4); // DWC_DDRPHYA_INITENG0_SequenceReg0b21s1
dwc_ddrphy_apb_wr(0x9006a,0x18); // DWC_DDRPHYA_INITENG0_SequenceReg0b21s2
dwc_ddrphy_apb_wr(0x9006b,0x0); // DWC_DDRPHYA_INITENG0_SequenceReg0b22s0
dwc_ddrphy_apb_wr(0x9006c,0x4); // DWC_DDRPHYA_INITENG0_SequenceReg0b22s1
dwc_ddrphy_apb_wr(0x9006d,0x78); // DWC_DDRPHYA_INITENG0_SequenceReg0b22s2
dwc_ddrphy_apb_wr(0x9006e,0x549); // DWC_DDRPHYA_INITENG0_SequenceReg0b23s0
dwc_ddrphy_apb_wr(0x9006f,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b23s1
dwc_ddrphy_apb_wr(0x90070,0x159); // DWC_DDRPHYA_INITENG0_SequenceReg0b23s2
dwc_ddrphy_apb_wr(0x90071,0xd49); // DWC_DDRPHYA_INITENG0_SequenceReg0b24s0
dwc_ddrphy_apb_wr(0x90072,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b24s1
dwc_ddrphy_apb_wr(0x90073,0x159); // DWC_DDRPHYA_INITENG0_SequenceReg0b24s2
dwc_ddrphy_apb_wr(0x90074,0x94a); // DWC_DDRPHYA_INITENG0_SequenceReg0b25s0
dwc_ddrphy_apb_wr(0x90075,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b25s1
dwc_ddrphy_apb_wr(0x90076,0x159); // DWC_DDRPHYA_INITENG0_SequenceReg0b25s2
dwc_ddrphy_apb_wr(0x90077,0x441); // DWC_DDRPHYA_INITENG0_SequenceReg0b26s0
dwc_ddrphy_apb_wr(0x90078,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b26s1
dwc_ddrphy_apb_wr(0x90079,0x149); // DWC_DDRPHYA_INITENG0_SequenceReg0b26s2
dwc_ddrphy_apb_wr(0x9007a,0x42); // DWC_DDRPHYA_INITENG0_SequenceReg0b27s0
dwc_ddrphy_apb_wr(0x9007b,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b27s1
dwc_ddrphy_apb_wr(0x9007c,0x149); // DWC_DDRPHYA_INITENG0_SequenceReg0b27s2
dwc_ddrphy_apb_wr(0x9007d,0x1); // DWC_DDRPHYA_INITENG0_SequenceReg0b28s0
dwc_ddrphy_apb_wr(0x9007e,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b28s1
dwc_ddrphy_apb_wr(0x9007f,0x149); // DWC_DDRPHYA_INITENG0_SequenceReg0b28s2
dwc_ddrphy_apb_wr(0x90080,0x0); // DWC_DDRPHYA_INITENG0_SequenceReg0b29s0
dwc_ddrphy_apb_wr(0x90081,0xe0); // DWC_DDRPHYA_INITENG0_SequenceReg0b29s1
dwc_ddrphy_apb_wr(0x90082,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b29s2
dwc_ddrphy_apb_wr(0x90083,0xa); // DWC_DDRPHYA_INITENG0_SequenceReg0b30s0
dwc_ddrphy_apb_wr(0x90084,0x10); // DWC_DDRPHYA_INITENG0_SequenceReg0b30s1
dwc_ddrphy_apb_wr(0x90085,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b30s2
dwc_ddrphy_apb_wr(0x90086,0x9); // DWC_DDRPHYA_INITENG0_SequenceReg0b31s0
dwc_ddrphy_apb_wr(0x90087,0x3c0); // DWC_DDRPHYA_INITENG0_SequenceReg0b31s1
dwc_ddrphy_apb_wr(0x90088,0x149); // DWC_DDRPHYA_INITENG0_SequenceReg0b31s2
dwc_ddrphy_apb_wr(0x90089,0x9); // DWC_DDRPHYA_INITENG0_SequenceReg0b32s0
dwc_ddrphy_apb_wr(0x9008a,0x3c0); // DWC_DDRPHYA_INITENG0_SequenceReg0b32s1
dwc_ddrphy_apb_wr(0x9008b,0x159); // DWC_DDRPHYA_INITENG0_SequenceReg0b32s2
dwc_ddrphy_apb_wr(0x9008c,0x18); // DWC_DDRPHYA_INITENG0_SequenceReg0b33s0
dwc_ddrphy_apb_wr(0x9008d,0x10); // DWC_DDRPHYA_INITENG0_SequenceReg0b33s1
dwc_ddrphy_apb_wr(0x9008e,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b33s2
dwc_ddrphy_apb_wr(0x9008f,0x0); // DWC_DDRPHYA_INITENG0_SequenceReg0b34s0
dwc_ddrphy_apb_wr(0x90090,0x3c0); // DWC_DDRPHYA_INITENG0_SequenceReg0b34s1
dwc_ddrphy_apb_wr(0x90091,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b34s2
dwc_ddrphy_apb_wr(0x90092,0x18); // DWC_DDRPHYA_INITENG0_SequenceReg0b35s0
dwc_ddrphy_apb_wr(0x90093,0x4); // DWC_DDRPHYA_INITENG0_SequenceReg0b35s1
dwc_ddrphy_apb_wr(0x90094,0x48); // DWC_DDRPHYA_INITENG0_SequenceReg0b35s2
dwc_ddrphy_apb_wr(0x90095,0x18); // DWC_DDRPHYA_INITENG0_SequenceReg0b36s0
dwc_ddrphy_apb_wr(0x90096,0x4); // DWC_DDRPHYA_INITENG0_SequenceReg0b36s1
dwc_ddrphy_apb_wr(0x90097,0x58); // DWC_DDRPHYA_INITENG0_SequenceReg0b36s2
dwc_ddrphy_apb_wr(0x90098,0xa); // DWC_DDRPHYA_INITENG0_SequenceReg0b37s0
dwc_ddrphy_apb_wr(0x90099,0x10); // DWC_DDRPHYA_INITENG0_SequenceReg0b37s1
dwc_ddrphy_apb_wr(0x9009a,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b37s2
dwc_ddrphy_apb_wr(0x9009b,0x2); // DWC_DDRPHYA_INITENG0_SequenceReg0b38s0
dwc_ddrphy_apb_wr(0x9009c,0x10); // DWC_DDRPHYA_INITENG0_SequenceReg0b38s1
dwc_ddrphy_apb_wr(0x9009d,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b38s2
dwc_ddrphy_apb_wr(0x9009e,0x7); // DWC_DDRPHYA_INITENG0_SequenceReg0b39s0
dwc_ddrphy_apb_wr(0x9009f,0x7c0); // DWC_DDRPHYA_INITENG0_SequenceReg0b39s1
dwc_ddrphy_apb_wr(0x900a0,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b39s2
dwc_ddrphy_apb_wr(0x900a1,0x10); // DWC_DDRPHYA_INITENG0_SequenceReg0b40s0
dwc_ddrphy_apb_wr(0x900a2,0x10); // DWC_DDRPHYA_INITENG0_SequenceReg0b40s1
dwc_ddrphy_apb_wr(0x900a3,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b40s2
dwc_ddrphy_apb_wr(0x900a4,0x0); // DWC_DDRPHYA_INITENG0_SequenceReg0b41s0
dwc_ddrphy_apb_wr(0x900a5,0x8140); // DWC_DDRPHYA_INITENG0_SequenceReg0b41s1
dwc_ddrphy_apb_wr(0x900a6,0x10c); // DWC_DDRPHYA_INITENG0_SequenceReg0b41s2
dwc_ddrphy_apb_wr(0x900a7,0x10); // DWC_DDRPHYA_INITENG0_SequenceReg0b42s0
dwc_ddrphy_apb_wr(0x900a8,0x8138); // DWC_DDRPHYA_INITENG0_SequenceReg0b42s1
dwc_ddrphy_apb_wr(0x900a9,0x10c); // DWC_DDRPHYA_INITENG0_SequenceReg0b42s2
dwc_ddrphy_apb_wr(0x900aa,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b43s0
dwc_ddrphy_apb_wr(0x900ab,0x7c8); // DWC_DDRPHYA_INITENG0_SequenceReg0b43s1
dwc_ddrphy_apb_wr(0x900ac,0x101); // DWC_DDRPHYA_INITENG0_SequenceReg0b43s2
dwc_ddrphy_apb_wr(0x900ad,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b44s0
dwc_ddrphy_apb_wr(0x900ae,0x0); // DWC_DDRPHYA_INITENG0_SequenceReg0b44s1
dwc_ddrphy_apb_wr(0x900af,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b44s2
dwc_ddrphy_apb_wr(0x900b0,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b45s0
dwc_ddrphy_apb_wr(0x900b1,0x448); // DWC_DDRPHYA_INITENG0_SequenceReg0b45s1
dwc_ddrphy_apb_wr(0x900b2,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b45s2
dwc_ddrphy_apb_wr(0x900b3,0xf); // DWC_DDRPHYA_INITENG0_SequenceReg0b46s0
dwc_ddrphy_apb_wr(0x900b4,0x7c0); // DWC_DDRPHYA_INITENG0_SequenceReg0b46s1
dwc_ddrphy_apb_wr(0x900b5,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b46s2
dwc_ddrphy_apb_wr(0x900b6,0x47); // DWC_DDRPHYA_INITENG0_SequenceReg0b47s0
dwc_ddrphy_apb_wr(0x900b7,0x630); // DWC_DDRPHYA_INITENG0_SequenceReg0b47s1
dwc_ddrphy_apb_wr(0x900b8,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b47s2
dwc_ddrphy_apb_wr(0x900b9,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b48s0
dwc_ddrphy_apb_wr(0x900ba,0x618); // DWC_DDRPHYA_INITENG0_SequenceReg0b48s1
dwc_ddrphy_apb_wr(0x900bb,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b48s2
dwc_ddrphy_apb_wr(0x900bc,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b49s0
dwc_ddrphy_apb_wr(0x900bd,0xe0); // DWC_DDRPHYA_INITENG0_SequenceReg0b49s1
dwc_ddrphy_apb_wr(0x900be,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b49s2
dwc_ddrphy_apb_wr(0x900bf,0x0); // DWC_DDRPHYA_INITENG0_SequenceReg0b50s0
dwc_ddrphy_apb_wr(0x900c0,0x7c8); // DWC_DDRPHYA_INITENG0_SequenceReg0b50s1
dwc_ddrphy_apb_wr(0x900c1,0x109); // DWC_DDRPHYA_INITENG0_SequenceReg0b50s2
dwc_ddrphy_apb_wr(0x900c2,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b51s0
dwc_ddrphy_apb_wr(0x900c3,0x8140); // DWC_DDRPHYA_INITENG0_SequenceReg0b51s1
dwc_ddrphy_apb_wr(0x900c4,0x10c); // DWC_DDRPHYA_INITENG0_SequenceReg0b51s2
dwc_ddrphy_apb_wr(0x900c5,0x0); // DWC_DDRPHYA_INITENG0_SequenceReg0b52s0
dwc_ddrphy_apb_wr(0x900c6,0x1); // DWC_DDRPHYA_INITENG0_SequenceReg0b52s1
dwc_ddrphy_apb_wr(0x900c7,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b52s2
dwc_ddrphy_apb_wr(0x900c8,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b53s0
dwc_ddrphy_apb_wr(0x900c9,0x4); // DWC_DDRPHYA_INITENG0_SequenceReg0b53s1
dwc_ddrphy_apb_wr(0x900ca,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b53s2
dwc_ddrphy_apb_wr(0x900cb,0x8); // DWC_DDRPHYA_INITENG0_SequenceReg0b54s0
dwc_ddrphy_apb_wr(0x900cc,0x7c8); // DWC_DDRPHYA_INITENG0_SequenceReg0b54s1
dwc_ddrphy_apb_wr(0x900cd,0x101); // DWC_DDRPHYA_INITENG0_SequenceReg0b54s2
dwc_ddrphy_apb_wr(0x90006,0x0); // DWC_DDRPHYA_INITENG0_PostSequenceReg0b0s0
dwc_ddrphy_apb_wr(0x90007,0x0); // DWC_DDRPHYA_INITENG0_PostSequenceReg0b0s1
dwc_ddrphy_apb_wr(0x90008,0x8); // DWC_DDRPHYA_INITENG0_PostSequenceReg0b0s2
dwc_ddrphy_apb_wr(0x90009,0x0); // DWC_DDRPHYA_INITENG0_PostSequenceReg0b1s0
dwc_ddrphy_apb_wr(0x9000a,0x0); // DWC_DDRPHYA_INITENG0_PostSequenceReg0b1s1
dwc_ddrphy_apb_wr(0x9000b,0x0); // DWC_DDRPHYA_INITENG0_PostSequenceReg0b1s2
dwc_ddrphy_apb_wr(0xd00e7,0x400); // DWC_DDRPHYA_APBONLY0_SequencerOverride
dwc_ddrphy_apb_wr(0x90017,0x0); // DWC_DDRPHYA_INITENG0_StartVector0b0
dwc_ddrphy_apb_wr(0x90026,0x2c); // DWC_DDRPHYA_INITENG0_StartVector0b15
dwc_ddrphy_apb_wr(0x9000c,0x0); // DWC_DDRPHYA_INITENG0_Seq0BDisableFlag0
dwc_ddrphy_apb_wr(0x9000d,0x173); // DWC_DDRPHYA_INITENG0_Seq0BDisableFlag1
dwc_ddrphy_apb_wr(0x9000e,0x60); // DWC_DDRPHYA_INITENG0_Seq0BDisableFlag2
dwc_ddrphy_apb_wr(0x9000f,0x6110); // DWC_DDRPHYA_INITENG0_Seq0BDisableFlag3
dwc_ddrphy_apb_wr(0x90010,0x2152); // DWC_DDRPHYA_INITENG0_Seq0BDisableFlag4
dwc_ddrphy_apb_wr(0x90011,0xdfbd); // DWC_DDRPHYA_INITENG0_Seq0BDisableFlag5
dwc_ddrphy_apb_wr(0x90012,0xffff); // DWC_DDRPHYA_INITENG0_Seq0BDisableFlag6
dwc_ddrphy_apb_wr(0x90013,0x6152); // DWC_DDRPHYA_INITENG0_Seq0BDisableFlag7
// // Disabling Ucclk (PMU) and Hclk (training hardware)
dwc_ddrphy_apb_wr(0xc0080,0x0); // DWC_DDRPHYA_DRTUB0_UcclkHclkEnables
// // Isolate the APB access from the internal CSRs by setting the MicroContMuxSel CSR to 1. 
dwc_ddrphy_apb_wr(0xd0000,0x1); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel
// // [phyinit_I_loadPIEImage] End of dwc_ddrphy_phyinit_I_loadPIEImage()
// 
// 
// //##############################################################
// //
// // dwc_ddrphy_phyinit_userCustom_customPostTrain is a user-editable function.
// //
// // See PhyInit App Note for detailed description and function usage
// 
// //##############################################################
// 
//zhouwei dwc_ddrphy_phyinit_userCustom_customPostTrain ();

//----------------------------------------------------------------------
//  save 1d2d training CSR 
//----------------------------------------------------------------------
if(!after_retention){
//save_1d2d_trained_csr(SAVE_DDRPHY_TRAIN_ADDR);
}

// // [dwc_ddrphy_phyinit_userCustom_customPostTrain] End of dwc_ddrphy_phyinit_userCustom_customPostTrain()
// // [dwc_ddrphy_phyinit_userCustom_J_enterMissionMode] Start of dwc_ddrphy_phyinit_userCustom_J_enterMissionMode()
// 
// 
// //##############################################################
// //
// // 4.3.10(J) Initialize the PHY to Mission Mode through DFI Initialization 
// //
// // Initialize the PHY to mission mode as follows: 
// //
// // 1. Set the PHY input clocks to the desired frequency. 
// // 2. Initialize the PHY to mission mode by performing DFI Initialization. 
// //    Please see the DFI specification for more information. See the DFI frequency bus encoding in section <XXX>.
// // Note: The PHY training firmware initializes the DRAM state. if skip 
// // training is used, the DRAM state is not initialized. 
// //
// //##############################################################
// 
//zhouwei dwc_ddrphy_phyinit_userCustom_J_enterMissionMode ();

// 
// // [dwc_ddrphy_phyinit_userCustom_J_enterMissionMode] End of dwc_ddrphy_phyinit_userCustom_J_enterMissionMode()
// [dwc_ddrphy_phyinit_sequence] End of dwc_ddrphy_phyinit_sequence()
// [dwc_ddrphy_phyinit_main] End of dwc_ddrphy_phyinit_main()
}
void dwc_ddrphy_phyinit_userCustom_G_waitFwDone(){
    volatile unsigned int tmp, tmp_t, i;
    volatile unsigned int train_ok;
    volatile unsigned int train_fail;
    volatile unsigned int stream_msg;
    WORD stream_nb_args;
    WORD stream_index;
    WORD stream_arg_pos;
    WORD message;
    WORD stream_arg_val;
    WORD tmp_data;
train_ok = 0;
train_fail = 0;
stream_msg = 0;
while(train_ok==0 && train_fail==0)
{
  // read UctShadowRegs, bit[0] will be set to 0 (looking for 0) when PMU has a message
    tmp = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0004);
    tmp_t = tmp & 0x01;
    while(tmp_t){
      tmp = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0004);
      tmp_t = tmp & 0x01;
    }

  //read UctWriteOnlyShadow: 0x07 indicates success, 0xff indicates fail
  tmp = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0032);
  //printk("PMU major stream =0x%x\n",tmp);
  //info_trigger(0xd0032,tmp,0x0);
  if(tmp==0x08){
  stream_msg = 1;
  //read UctDatWriteOnlyShadow(streaming message): 
  //tmp_t = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0034);
  //tmp_t = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0034);



  reg32_write(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0031,0x0);

  do {
  tmp_t = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0004);
  }while((tmp_t & 0x1) == 0x0);
  reg32_write(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0031,0x1);

  do {
  tmp_t = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0004);
  }while((tmp_t & 0x1) == 0x1);

  // read_mbox_mssg
  stream_nb_args = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0032);

  // read_mbox_msb
  stream_index = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0034);
  stream_index = (stream_index << 16) | stream_nb_args;
  //printk("PMU stream_index=0x%x nb_args=%d\n",stream_index, stream_nb_args);
  //info_trigger(0xd0034,stream_index,0xFFFFFFFF);

  stream_arg_pos = 0;
  while(stream_nb_args > 0) {
       // Need to complete previous handshake first...
       reg32_write(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0031,0x0);
       //poll_mbox_from_uc(1);

  do {
  tmp_t = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0004);
  }while((tmp_t & 0x1) == 0x0);
  reg32_write(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0031,0x1);

  // Read the next argument...
  do {
  tmp_t = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0004);
  }while((tmp_t & 0x1) == 0x1);


  // read_mbox_mssg
  message  = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0032);
  // read_mbox_msb
  stream_arg_val = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0034);
  stream_arg_val = (stream_arg_val << 16) | message;
  //printk("PMU stream_arg[%d]=0x%x\n",stream_arg_pos, stream_arg_val);
  //info_trigger(0xd0034,stream_arg_val,0xFFFF0000);
      stream_nb_args--;
      stream_arg_pos++;
  }//while(stream_nb_args > 0)



  }
  else if(tmp==0x07)
  {
      train_ok = 1;   
      printf("\r\nDDR3 FW100 1600MTS ONE RANK TRAINING PASS\r\n");
  }
  else if(tmp==0xff)
  {
      train_fail = 1; 
      printf("\r\nDDR3 FW100 1600MTS ONE RANK TRAINING FAILED\r\n");
  }
  else  {train_ok = 0; train_fail = 0; stream_msg = 0;}

  //Write the DctWriteProt to 0 to ack the receipt of the message.
  reg32_write(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0031,0x0);

  if(stream_msg == 1){
  tmp_t = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0034);
  tmp_t = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0034);
  }

  //Poll the UctWriteProtShadow, looking for 1
    tmp = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0004);
    tmp_t = tmp & 0x01;
    while(tmp_t==0){
    tmp = reg32_read(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0004);
    tmp_t = tmp & 0x01;
    }
  //When a  1 is seen, Write the DctWriteProt to 1 to complete the protocol.
  reg32_write(IP2APB_DDRPHY_IPS_BASE_ADDR(0)+4*0xd0031,0x1);
}
}

