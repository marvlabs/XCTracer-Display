/* Draw the XCTracer values on the LCD
 * 
      #         ###    #####        ###        ###          #     ###               
     ##        ####   #######      ####       #   #        ##    #         #####    
  #####        ####   ###  ###     ####       #  ##   #     #   #          #   #    
  #####       # ###  ###          # ###       # # #         #   ####       #   #    
    ###       # ###  ### ###      # ###       ##  #   #     #   #   #      #   #    
    ###      #  ###  ########    #  ###       #   #         #   #   #      #   #    
    ###      #  ###  #### ####   #  ###        ###         ###   ###       #   #    
    ###     #   ###  ###   ###  #   ###                                    #   #    
    ###     ######## ###   ###  ########                                   #   #    
    ###     ######## ###   ###  ########        #      #    ###    #       #   #    
    ###         ###   ### ####      ###        ##     ##   #      ##       #   #    
    ###         ###   #######       ###         #    # #  #      # #       #   #    
    ###         ###    #####        ###         #   #  #  ####  #  #       #####    
                                                #   ##### #   # #####      #####    
                                                #      #  #   #    #       #####    
                                               ###     #   ###     #       #####    
                                                                           #####    
                                                                           #####    
   ####   #########                               #          #####         #####    
  ####### #########                              ##         #######        #####    
 ###  ###       ###                           #####        ###   ###     ########## 
 ###  ###      ###                            #####        ###   ###       #   #    
      ###      ##                               ###              ###       #   #    
     ##       ###                    ###        ###              ###       #   #    
     ###      ##                     ###        ###             ###        #   #    
      ###    ###                     ###        ###            ###         #   #    
      ###    ###                   #######      ###           ###          #   #    
 ###  ###    ##                    #######      ###          ###           #   #    
 ###  ###   ###                      ###        ###         ###            #   #    
  ######    ###                      ###        ###    ###  ########       #   #    
   ####     ###                      ###        ###    ###  ########       #   #    
#                 #                                                        #   #    
#               # #                                          #             #   #    
#  #  ## #     #  # ##                             ## #     #   ####       #   #    
# #   # # #   #   ##  #                            # # #   #   #           #   #    
##    # # #  #    #   #                            # # #  #     ###        #   #    
# #   # # # #     #   #                            # # # #         #       #   #    
#  #  # # #       #   #                            # # #       ####        #   #    
                                                                           #   #    
                                                                           #####    
                                                                                    
                                        #####        ###   ###  #   #  #   ## ### # 
                   #                        #       #   # #   # #   #   ### # ### # 
#######################################    #        #   #     # #   #   ### #  ## # 
#             ######                  #   ##         ###   ###  #   #  #   ## # # # 
#             ######                  #     #       #   # #     #   #   ### # ##  # 
####################################### #   #   ##  #   # #      # #    ### # ### # 
                   #                     ###    ##   ###  #####   #    #   ## ### # 


 * (C) by Marcel Bucher 2016, MIT LICENCSE
 */

#ifndef LCD_GUI_H
#define LCD_GUI_H
/////////////////////////////
// Constants to control GUI

#define LCD_CONTRAST   40	// Good contrast setting for 5110

#define VMAXCLIMB       3.0	// 100% value for vario bar
#define VAVERAGE       40.0	// Middle value for speed bar
#define VMAXDIFF       10.0	// 100% difference value speed bar

// Values
#define POS_HBARO  0,12
#define POS_HGPS  46, 9
#define POS_VARIO 33,30
#define POS_SPEED  0,30
#define POS_TIME  46, 0

#define POS_KMH    0,31
#define POS_MS    51,31
#define POS_VOLT  40,41
#define POS_SAT   71,41
#define POS_STATUS 0,32


// Vario Bar
#define VX 75
#define VW 5
#define VY 1
#define VH 39
#define VBARY VY + (VH-1)/2
#define VBAROVERHANG 2
#define VBARX VX-VBAROVERHANG
#define VBARW VW+2*VBAROVERHANG
#define VBARX2 VBARX+VBARW

// Speed Bar
#define SX 0
#define SW 39
#define SY 43
#define SH 4
#define SBARX SX + (SW-1)/2
#define SBAROVERHANG 1
#define SBARY SY-SBAROVERHANG
#define SBARH SH+2*SBAROVERHANG
#define SBARY2 SBARY+SBARH


/////////////////////////////
extern void InitLcd();
extern void DrawGui(int Vario, byte Speed, int HeightGps, int HeightBaro, byte Satellites, const String& Status, float VRaw, char Recording);

// DEBUG only
extern void PrintScreenToSerial();

/////////////////////////////
#endif
