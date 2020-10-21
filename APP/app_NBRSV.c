/*
*******************************************************************************
*
*                           the app_NBRSV file
*                  ST Microelectronics STM32 with the STM32L151
*
* Filename      : app_NBRSV.c
* Version       : V1.0.0
* Programmer(s) : wubo
*******************************************************************************
*/

#include "includes.h"


#define TXDFLAG     0x00f0
#define RXDFLAG     0x000f


const       uint8_t     NBACK[][16] =   {
                                            "+CSCON",       // 濠电姷鏁搁崕鎴犲緤閽樺娲偐鐠囪尙鍔﹀銈嗗坊閸嬫捇鏌涢悢绋款嚋缂佺粯鐩畷鍫曨敆娴ｅ弶瀚兼俊鐐€栭崝褏寰婇崸妞尖偓鍌炲蓟閵夛妇鍘搁柣蹇曞仩婵倝宕板鈧弻鐔碱敊鐟欏嫭鐝氶梺纭呮珪缁酣宕版繝鍋界喐娼弶娆炬闂傚倷娴囧▔鏇㈠闯閿曞倸绠柨鐕傛嫹                                                                          
                                            "+CEREG",       // 闂傚倸鍊峰ù鍥ㄧ珶閸喆浠堢紒瀣儥濞兼牠鏌ゆ慨鎰偓妤冪矆婵犲偆鐔嗛柤鎼佹涧婵洨绱掗悪鍛М闁哄瞼鍠栭弻鍥晝閳ь剚鏅堕濮愪簻妞ゆ劑鍊曞瓭闂佸疇妫勯ˇ鐢哥嵁濮椻偓楠炲海绮电€Ｑ冧壕婵°倕鎳忛悡娆愩亜閺嶃儱鈧牜鈧熬鎷�                         
                                            "+CGATT",       // 婵犵數濮烽。钘壩ｉ崨鏉戝瀭妞ゅ繐鐗嗛悞鍨亜閹烘垵鏆為柣婵愪邯閺屾稓鈧絻鍔岄崝婊呯磼缂佹绠橀柛鐘诧工铻ｉ弶鐐存緲椤ユ艾鈹戦悩鍨毄闁稿濮锋禍鎼侇敂閸偅鏅╂繝鐢靛Т鐎氼喚绮婚弮鍌楀亾閸忓浜鹃梺鍛婃处閸嬪顤傞梻鍌欐祰濞夋洟宕抽敃鍌氱闁跨噦鎷�
                                            "+NSOCLI",      // 闂傚倸鍊烽懗鑸电仚缂備胶绮崹鍓佹崲濞戞瑧绡€闁稿濮ょ粙鎾诲窗婵犲洤鐒炬い銈咁攺ET濠电姷鏁搁崕鎴犲緤閽樺娲偐鐠囪尙顦┑鐘绘涧濞层倝顢氶柆宥嗙叆闁绘洖鍊圭€氾拷
                                            "CONNECT OK",   // 闂傚倷绀侀幖顐λ囬锕€鐤炬繝濠傛噹閸ㄦ繈骞栧ǎ顒€濡奸柣鎾寸箞閺屾稖绠涢幘铏€繛瀛樺殠閸婃繈寮婚敐澶婄疀妞ゆ挾鍠撶粙鍥ㄧ節閵忥絾纭剧紒澶婄秺瀵鎮㈢悰鈥充壕婵炴垶顏鍛弿闁搞儜鈧弨鑺ャ亜閺囩偞顥為悗姘炬嫹
                                            "+CME ERROR",   // 婵犵數濮烽。钘壩ｉ崨鏉戝瀭妞ゅ繐鐗嗛悞鍨亜閹烘垵鏆為柣婵愪邯閺屾稓鈧絻鍔岄崝锕傛煛瀹€鈧崰鏍偂椤掑嫬绠甸柟鐑樺灥琚濋梻鍌欐祰濡椼劎娆㈤妶澶婄闁跨噦鎷�                          
                                            "ERROR",        // 闂傚倸鍊风粈浣革耿鏉堛劎浠氭繝鐢靛仩椤曟粎绱炴笟鈧悰顔芥償閵娿儙褍顭跨捄鐚村姛妞わ腹鏅涢埞鎴︽倷閸欏鏋欐繛瀛樼矋缁诲牆顕ｉ弻銉︽櫢闁跨噦鎷�                                  
                                            "+NSOSTR",      // 闂傚倸鍊峰ù鍥ь浖閵娾晜鍤勯柤绋跨仛濞呯姵淇婇妶鍌氫壕闂佷紮绲介悘姘辩箔閻旂厧鐒垫い鎺嗗亾闁伙絿鍏橀弫鎰緞婵犲嫷鍚呴梻浣瑰濮婂骞婇幘瀵割洸闁诡垼鐏愯ぐ鎺撴櫜闁搞儯鍔屽▓宀勬⒑鐠囪尙绠版繛宸弮瀵鈽夐姀鐘电潉闂佸湱鍋撳娆擃敂閿燂拷
                                            "+NSORF",       // 闂傚倸鍊峰ù鍥ь浖閵娾晜鍤勯柤绋跨仛濞呯姵淇婇妶鍌氫壕闂佷紮绲介悘姘辩箔閻旂厧鐒垫い鎺戝瀹撲線鏌熼悜姗嗘當閸烆垶姊洪幐搴㈩梿濠殿喓鍊曡闁糕剝绋掗埛鎴︽煟閻旂ǹ顥嬮柟鐣屽█閺屾盯寮埀顒€煤濮椻偓楠炲牓濡搁埡鍌涙闂佽法鍣﹂幏锟�                                
                                            "+CSQ",         // 濠电姷鏁搁崕鎴犲緤閽樺娲偐鐠囪尙鍔﹀銈嗗坊閸嬫捇鏌涢悢绋款嚋缂佺粯鐩畷鍫曨敆娴ｉ晲鍖栭梻浣筋潐婢瑰寮插☉娆愬弿闁稿瞼鍋為悡鏇㈡煥濠靛棛绠查悗姘炬嫹
                                            "+CCLK",        // 濠电姷鏁搁崕鎴犲緤閽樺娲偐鐠囪尙鍔﹀銈嗗坊閸嬫捇鏌涢悢绋款嚋缂佺粯鐩畷鍫曨敆娴ｅ搫骞愬┑鐐舵彧缁蹭粙骞栭锔衡偓鍌炴嚃閳规儳浜鹃悷娆忓閹牓鏌熼崙銈嗗
                                            "460"          // ISMI闂傚倸鍊风粈渚€骞夐敓鐘冲仭闂侇剙绉寸粻顖炴煥閻曞倹瀚�                    
                                        };    
                                

static      uint8_t     ucom[UART2_RX_BUF_SIZE]         = {0};      // ???????
#if 0
static      uint8_t     datbuf[UART2_TX_BUF_SIZE]       = {0};      // hex?asc???
#endif
static      uint8_t     gucNbCard[8]        = {0};      // NB?????


__IO        STUNBSTA    gstuNbSta;                      // NB???
__IO        uint8_t     gucCsqValue         = 0;        // ????
  

/*******************************************************************************
* Function Name  : At_SendStr
* Description    : At????   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t Asc2Hex(uint8_t uc)
{
    uint8_t uct = 0;
	
	uct = uc;	
	if(uct >= 0x30 && uct <= 0x39)      {   // 0 ~ 9
		uct -= 0x30;
	}
	else if(uct >= 0x41 && uct <= 0x46) {   // A ~ F
        uct -= 0x37;	
	}	
	else {
		uct = 0;
	}		         
    return uct;
}

/*******************************************************************************
* Function Name  : AscReConv
* Description    : ??NBiot??????,????hex?? 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t AscReConv(uint8_t *src,uint8_t *dst,uint16_t cnt)
{	
    uint16_t    length 		= 0;
	uint16_t 	u2tmp[2] 	= {0};
	uint16_t    i = 0, 	    j = 0;	
	uint8_t     hibit = 0, 	lobit = 0;
	    
	for(i = 0;i < cnt;i++)
	{   	
		if((src[i] == 0x33 && src[i+1] == 0x38) || (src[i] == 0x37 && src[i+1] == 0x42))
		{   // 0x38, 0x7b	
			hibit 	= Asc2Hex(src[i+2]) & 0x0f;
			lobit 	= Asc2Hex(src[i+3]) & 0x0f;					
			u2tmp[0] = (uint16_t)((hibit << 4) + lobit);

			hibit 	= Asc2Hex(src[i+4]) & 0x0f;
			lobit 	= Asc2Hex(src[i+5]) & 0x0f;					
			u2tmp[1] = (uint16_t)((hibit << 4) + lobit);
			
			length 	= 4 + u2tmp[0] + (u2tmp[1]<<8);     
            
			if(src[i+length*2-2] == 0x37 && src[i+length*2-1] == 0x44)     
			{   // 0x7d
				for(j = 0;j < length;j++)
				{
					hibit = Asc2Hex(src[i+j*2]) & 0x0f;
					lobit = Asc2Hex(src[i+j*2+1]) & 0x0f;									
					dst[j] = (hibit << 4) + lobit;					
				}
				return length;	// ?????
			} 			
		}
	}
	return 0;
}


/*******************************************************************************
* Function Name  : Parch
* Description    : At????   
* Input          : ??????
* Output         : pch???????,c??????
* Return         : None
*******************************************************************************/
#if 0
void Parch(uint8_t *pch,uint16_t c)
{   
	uint16_t    len = 0;
    uint8_t     uct = 0;
    

    STUCLOCKSYNC mstuClock;
    
    len = AscReConv(pch,datbuf,c);          // ???????    
    
	if(len > 3)                             // for(i = 0;i < len;i++)
    {
        if(datbuf[0] == 0x38 || datbuf[0] == 0x7B)
        {            
			//length = (uint16_t)datbuf[1] + ((uint16_t)datbuf[2]<<8);
			
            if(datbuf[len-1] == 0x7D)       // ??????
            {                       
                switch(datbuf[3])
                {
                    case 0xA5:	// ???????
                        break;
                    
                    case 0x81:	// ??????
						//xSemaphoreGive(xSem_NbDataReq);																  
                        break;
				
                    case 0x83:	// ??????
                        uct = NbiotSum((uint8_t *)datbuf + 1, len - 3);     
                        if(uct == datbuf[len-2])
                        {
                            memcpy((uint8_t *)&tstuWmTab,  (uint8_t *)datbuf,  len);                            
                            //xQueueSendToBack(xQue_WmTab,( void * )&tstuWmTab,0);                                                                
                        }
                        break;
                  						
                    case 0x15: 	//????  
                        uct = NbiotSum((uint8_t *)datbuf + 1, len - 3);                        
                        if(uct == datbuf[len-2])
                        {
                            memcpy((uint8_t *)&mstuClock,   (uint8_t *)datbuf,  len);                                                  
                            //xQueueSendToBack(xQue_RTC,( void * )&mstuClock,0);           
                        }
                        break;
					
                    default:
                        break;
                }					
            }
        }       
    }
}
#endif	

/*******************************************************************************
* Function Name  : GerNbClock
* Description    : ??NB????(????8??)
* Input          : pch???????,c??????
* Output         : None
* Return         : None
*******************************************************************************/
/*
+CCLK:19/04/19,02:26:51+32
*/
void GerNbClock(uint8_t *pch,uint16_t c)
{	    
    TTime       mRtcTime;       
	uint8_t     hibit = 0, 	lobit = 0;
    uint8_t 	uct = 0;
   
    if(pch[23] == '+')  // ??????
    {
        hibit 	= Asc2Hex(pch[6]) & 0x0f;
        lobit 	= Asc2Hex(pch[7]) & 0x0f;					
        uct     = ((hibit * 10) + lobit);    
        mRtcTime.year = uct;
          
        hibit 	= Asc2Hex(pch[9]) & 0x0f;
        lobit 	= Asc2Hex(pch[10]) & 0x0f;					
        uct     = ((hibit * 10) + lobit);
        mRtcTime.month = uct;
        
        hibit 	= Asc2Hex(pch[12]) & 0x0f;
        lobit 	= Asc2Hex(pch[13]) & 0x0f;					
        uct     = ((hibit * 10) + lobit);
        mRtcTime.day = uct;
        
        hibit 	= Asc2Hex(pch[15]) & 0x0f;
        lobit 	= Asc2Hex(pch[16]) & 0x0f;					
        uct     = ((hibit * 10) + lobit);
        mRtcTime.hours = uct;
        
        hibit 	= Asc2Hex(pch[18]) & 0x0f;
        lobit 	= Asc2Hex(pch[19]) & 0x0f;					
        uct     = ((hibit * 10) + lobit);
        mRtcTime.minutes = uct;
        
        hibit 	= Asc2Hex(pch[21]) & 0x0f;
        lobit 	= Asc2Hex(pch[22]) & 0x0f;					
        uct     = ((hibit * 10) + lobit);
        mRtcTime.seconds = uct;
               
        Set_Time(mRtcTime);
    }
}


/*******************************************************************************
* Function Name  : MsgDecode
* Description    : ??NB?????   
* Input          : pch???????,c??????
* Output         : None
* Return         : None
*******************************************************************************/
static uint16_t  MsgDecode(uint8_t *pch,uint16_t c)
{    
    uint16_t status = 0;
    
    uint8_t i = 0,  j = 0;
    uint8_t row = 0,len=0;        
    
    uint8_t uct = 0;
    //==============================================//
    if(pch[0] == 'O' && pch[1] == 'K')          // ??"OK"
    {
        DISABLE_INT();  
        gstuNbSta.ackok = 1;                    // ????                
        ENABLE_INT();
        return 0;
    }
    //==============================================//
    row = sizeof(NBACK) / sizeof(NBACK[0]);     // ?????     
    for(i = 0; i < row; i++)
    {                 
        len =  strlen((char *)NBACK[i]);        // ??????
        for(j = 0; j < len; j++)
        {
            // ??????NB??????
            if(pch[j] != NBACK[i][j])
            {
                break;
            }                    
        }
        
        if(j == len)        // ????
        {          
            row = i;        // ?????            
            status = 0x5555;// ??????
            break;
        }
    }               
 	//==============================================//
    if(status == 0x5555)    // ?????????
    {      
        DISABLE_INT();         
        switch(row)
        {
            case 0: //"+CSCON",        
                uct = pch[c-5];                 // ??闂傚倸鍊烽懗鍫曘€佹繝鍌滄殾闁汇垻枪缁犵喖鏌￠崼銏╂綗闂傚倸鍊峰ù鍥р枖閺囥垹闂柨鏇炲€哥粻顖炴煥閻曞倹瀚�,???5???
                if(uct == '1' ) 
                {                    
                    gstuNbSta.cnnct = 1;        // ???(?????)
                    gstuNbSta.cscon = 1;        // ??????  
                }
                else 
                {
                    gstuNbSta.cnnct = 0;        // ???(?????)
                    gstuNbSta.cscon = 0;        // ?????
                }                                               
                break;
                                 
            case 1: //"+CEREG",   
                uct = pch[c-3];                 // ??闂傚倸鍊烽懗鍫曘€佹繝鍌滄殾闁汇垻枪缁犵喖鏌￠崼銏╂綗闂傚倸鍊峰ù鍥р枖閺囥垹闂柨鏇炲€哥粻顖炴煥閻曞倹瀚�,???3???
                if(uct == '1' || uct == '5') {
                    gstuNbSta.cereg = 1;        // ????
                }
                else {
                    gstuNbSta.cereg = 0;
                }                   
                break;
                                  
            case 2: //"+CGATT",
                 uct = pch[c-3];                // ??闂傚倸鍊烽懗鍫曘€佹繝鍌滄殾闁汇垻枪缁犵喖鏌￠崼銏╂綗闂傚倸鍊峰ù鍥р枖閺囥垹闂柨鏇炲€哥粻顖炴煥閻曞倹瀚�,???3???
                if(uct == '1')  {
                    gstuNbSta.cgatt = 1;        // ????
                }
                else {                
                    gstuNbSta.cgatt = 0;
                }
                break;
            
            case 3: //"+NSOCLI"                 // NB婵犵數濮烽。钘壩ｉ崨鏉戝瀭妞ゅ繐鐗嗛悞鍨亜閹烘垵鏆為柣婵愪邯閺屾稓鈧絻鍔岄崝锕傛煛瀹€鈧崰鎾舵閹烘嚦鐔煎传閸曞灚缍嶉梻鍌欑窔濞佳兠洪敃鍌ゆ晢闁荤姷饪眐et,闂傚倷娴囧畷鐢稿磻閻愮數鐭欓煫鍥ㄧ☉缁€澶愬箹濞ｎ剙濡煎鍛攽椤旇鐟扳枖濞戞氨灏电€广儱顦伴悡鐔镐繆椤栨繃顏犻柨娑樼Ч閺屾洟宕卞Δ瀣惈閻庢鍠栭悥濂稿春閿熺姴绀冮柕濞垮€栭崟鍐⒑濮瑰洤鐒洪柛銊ゅ嵆閹椽濡搁妷顔惧姺闁荤姴娲︾粊鎾醇椤忓牊鐓曟繛鎴濆綁缁ㄥジ鏌ｉ幘璺烘P缂傚倸鍊搁崐鐑芥倿閿曗偓铻為煫鍥ㄧ☉閸ㄥ倿鏌熷畡鎷岊潶濞存粌缍婇弻銊╂偆閸屾稑顏�                     
                gstuNbSta.nsocl = 1;            // ??
                break;
            
            case 4: //"CONNECT OK"                               
                gstuNbSta.nsoco = 1;            // 闂傚倷绀侀幖顐λ囬锕€鐤炬繝濠傛噹閸ㄦ繈骞栧ǎ顒€濡奸柣鎾寸箞閺屾稖绠涢幘铏€繛瀛樺殠閸婃繈寮婚敐澶婄疀妞ゆ挾鍠撶粙鍥ㄧ節閵忥絾纭剧紒澶婄秺瀵鎮㈢悰鈥充壕婵炴垶顏鍛弿闁搞儜鈧弨鑺ャ亜閺囩偞顥為悗姘炬嫹         
                break;
            
            case 5: //"+CME ERROR"              // 闂傚倸鍊烽懗鍫曘€佹繝鍕濞村吋娼欑壕鍧楁煟閵忊懚褰掑几娓氣偓閺屻劑鎮ら崒娑橆伓   
                gstuNbSta.nsocl = 1;            // 闂傚倸鍊搁崐鐑芥倿閿曚降浜归柛鎰典簽閻捇鏌熺紒銏犳灈缂佺姵宀搁弻銊╂偆閸屾稑顏�
                gstuNbSta.ackerr = 1;           // err闂傚倷绀佸﹢閬嶅储瑜旈幃娲Ω閵夘喗缍庢繝鐢靛У绾板秹宕戦妶澶嬬叆闁绘洖鍊圭€氾拷                        
                break;            
            
            case 6: //"ERROR"                   // 闂傚倸鍊风粈浣革耿鏉堛劎浠氭繝鐢靛仩椤曟粎绱炴笟鈧悰顔芥償閵娿儙褍顭跨捄鐚村姛妞わ腹鏅涢埞鎴︽倷閸欏鏋欐繛瀛樼矋缁诲牆顕ｉ弻銉ョ厸闁告侗鍠氶崢鐢告煟鎼淬垻鈯曟い顓炴处閺呭爼鎮介崨濠勫幈闂佹寧绻傞幊蹇涘疮閻愮鍋撻崹顐ｇ凡閻庢矮鍗冲顐﹀箻缂佹ɑ娅㈤梺璺ㄥ櫐閹凤拷          
                gstuNbSta.ackerr = 1;         
                break;
                 
            case 7: //"+NSOSTR"                 // nb????
                status = TXDFLAG;               
                gstuNbSta.nbtxd = 1;          
                break;
            
            case 8: //"+NSORF"                  // nb????
                status = RXDFLAG;   
                gstuNbSta.nbrxd = 1;                 
                break;   
          
            case 9: //"+CSQ"                    // ????                           
                uct = Asc2Hex(pch[5]);
                if(pch[6] >= 0x30 && pch[6] <= 0x39) // ???????
                {                
                    uct *= 10;
                    uct += Asc2Hex(pch[6]);		
                } 
                gucCsqValue = uct;              // ????
                gstuNbSta.csq = 1;            
                break; 
              
            case 10://"+CCLK"                    // ????                
                gstuNbSta.cclk = 1;                         
                
                GerNbClock(pch,c);              // ??NB????                                                                        
                break;   
                
            case 11://"cimi"                    // ????(460040101900126)
                gstuNbSta.cimi = 1;  

                gucNbCard[0] = 0x04;
                gucNbCard[1] = 0x60;
                for(i = 2; i < 8; i++)
                {
                    gucNbCard[i] =  (pch[2*i-1]<<4);
                    gucNbCard[i] += (pch[2*i]&0x0f);
                }
                break;     
            
            default:
                break;        
        }   
        ENABLE_INT();        
    }
    else     // ????
    {
        status = 0;                                   
    }    
    return status;
}


/*******************************************************************************
* Function Name  : GetCardImsi
* Description    : ????
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GetCardImsi(void)
{
    uint8_t i = 0;
    for(i = 0; i < 8; i++) {            // ????
        gstuNbPara.mu1Imsi[i] = gucNbCard[i];   
    }
    gstuNbPara.mu1Csq = gucCsqValue;    // ????
    gstuNbPara.mu1Check = CheckSum((uint8_t *)&gstuNbPara,(uint16_t)(sizeof(STUNBSET) - 1));                          
}

/*******************************************************************************
* Function Name  : Nb_Data_Rec
* Description    : ???NB?????
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Nb_Data_Rec(void)
{
    uint16_t count = 0;  
    uint16_t status = 0;
    
    count = myMemCpy(NB_COM,ucom);	

    if(count > 2)                           // ????"\r\n"
    {		                                             
        status = MsgDecode(ucom,count);	    // ??COM???

        if(status == RXDFLAG) 
        {                     
            //Parch(ucom,count);              // ??????  
        }
        
#if     DBGMODE                   
        if(count <= UART1_TX_BUF_SIZE) 
        {                   
            comSendBuf(DBGCOM,ucom,count);  // ????                                       
        }
#endif 
    }
}


static  uint16_t  NbRsv_len1 = 0;
static  uint16_t  NbRsv_len2 = 0;

/*******************************************************************************
* Function Name  : ProcNbComRec
* Description    : ???NB?????
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ProcNbComRec(void)
{
    NbRsv_len2 = GetRxCount(NB_COM);                           
    if((NbRsv_len1 > 0) && (NbRsv_len1 == NbRsv_len2))	    // COM????
    {									
        if(NbRsv_len1 <= UART2_RX_BUF_SIZE)                 // ???????
        {	                       
            Nb_Data_Rec();										
        }													
    }	  
    NbRsv_len1 = NbRsv_len2;//GetRxCount(NB_COM);	  
}      
/*******************************(END OF FILE)**********************************/
