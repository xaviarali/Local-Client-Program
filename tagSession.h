#include <stdio.h>
#include <windows.h>



#pragma pack(4)
#define MAXLEN_ENQREPLY       48
#define MAX_CBKEY             30  // Anzahl Zeichen für FKey Label
typedef struct   {
                  char   ID[4];

                  /*
                   * konstante Vorgaben
                   */
                  char   SessionName[260]; // MAXPATH
                  char   HostName   [260]; // MAXPATH
                  char   TermName   [MAXLEN_ENQREPLY]; // default= ansi
                  char   AnswerBack [MAXLEN_ENQREPLY]; // default=""

                  BYTE   StartAttr;              // default= 15
                  BYTE   TabSize;                // default=  8
                  WORD   tcp_port;               // default= 23
                  WORD   zeilen;                 // default= 25
                  WORD   spalten;                // default= 80
                  BOOL   fBeepOnConnect;         // default= TRUE;
                  BOOL   obsolete_fAutoConnect;  // default= TRUE
                  BOOL   fStartMaximized;        // default= FALSE, ausser bei 640x480
                  BOOL   fWinAttrMode;           // default= FALSE
                  BOOL   fVisualBEL;             // default= FALSE
                  char   old_F_Key[46][8];

                  /*
                   * während der Sitzung veränderbar;
                   * deshalb in 'vscreen' doppelt geführt
                   */
                  WORD   CursorType;             // CTP_UNDERLINE | CTP_BLOCK | CTP_LEFTLINE
                  WORD   CursorMode;             // CM_BLINK | CM_STEADY
                  BYTE   prn_LPI;                // default=  6
                  BYTE   prn_CPI;                // default= 12
                  BYTE   prn_PageLength;         // default= 64

                  // V 1.50 ->
                  BOOL   fMenuSession;
                  BOOL   fToolbar;
                  BOOL   fSendLogin;
                  BOOL   fSendPasswd;
                  char   LoginRequest[16];
                  char   PasswdRequest[16];
                  char   old_UserID[24];
                  char   old_Passwd[24];

                  // V 1.60 ->
                  BOOL   fSendCR;
                  BOOL   fPreferFixedFont;
                  int    FontCX;
                  int    FontCY;
                  int    PosX;
                  int    PosY;
                  char   ImagePath[260]; // MAXPATH

                  // V1.70 ->
                  BOOL   fSaveScreen;
                  BOOL   fKeepAlive;
                  WORD   nEnv;
                  char   Environment[512];

                  // V1.80 ->
                  WORD   HistoryLines;
                  BOOL   fRuler;
                  char   FaceName[LF_FACESIZE];
                  char   fBold;
                  char   fLocalEcho;

                  // V2.40 ->
                  char    PrinterName[260]; // MAXPATH
                  char    PrinterFont[LF_FACESIZE];
                  BOOL    fSessionLogging;
                  BOOL    fSelectedFolder;
                  char    FolderPath[260]; // MAXPATH

                  // V2.50 ->
                  int     XnCmdShow; // Obsolete

                  // V2.60
                  BOOL    fFunctionKeys;
                  char    FLabel[36][ MAX_CBKEY ];

                  // V2.70.02
                  BOOL    fAppendFormfeed;

                  // V3.00
                  BOOL    fQuickMouse;
                  BOOL    noAutoEOLwrap;
                  BOOL    fHupcl;

                  // V4.0
                  int     InitialRunMode;
                  BOOL    fNoContext;
                  BOOL    fNoDragDrop;

                  // Build 02
                  char    old_DEL_Key[8];

                  // Build 03
                  char    ExtFLabel[12][ MAX_CBKEY ];
                  char    seq_CKey [12][ MAX_CBKEY ];
                  char    seq_FKey [48][ MAX_CBKEY ];
                  #define LEFT_Key    seq_CKey[ 3]
                  #define DEL_Key     seq_CKey[10]
                  #define ESCAPE_Key  seq_CKey[11]

                  // 4.5  Build 01
                  DWORD   ConnectionTimeStamp;

                  // 4.5  Build 03
                  BOOL    ScoAnsi;
                          // 0=default, DMORIENT_LANDSCAPE, DMORIENT_PORTRAIT
                  short   PrintPage_Orientation;

                  // V5.0
                  WORD    prnLeftMargin;
                  WORD    prnTopMargin;
                  int     KeepAliveSec;
                  BOOL    fTransparentPrinting;
                  BOOL    fCenterSessionWindow;
                  BOOL    fWinLogon;

                  // V5.5
                  int     Protocol;

                  // V6.0
                  char    seq_C2Key [4][ MAX_CBKEY ];
                  #define TAB_Key         seq_C2Key[0]
                  #define BACKSPACE_Key   seq_C2Key[1]
                  #define ENTER_Key       seq_C2Key[2]
                  #define UNASSIGNED_Key  seq_C2Key[3]
                  struct  {
                            BYTE  keyboard[256];
                            BYTE  screen  [256];
                            BYTE  printer [256];
                          }
                          mapping;
                  BOOL    fConvertNumericDecimal; // obsolet
                  UINT    PrinterTimeout;

                  // V6.0 Build >= 617
                  DWORD   Relevance;

                  // V6.2
                  BOOL    fNoXButton;

                  // V7.0
                  BOOL    fPrinterSetup;
                  char    UserID[128];
                  char    Passwd[128];
                  char    SSH_keyfilename[260]; // MAXPATH

                  // v7.1
                  BOOL    fBoldPrinterFont;

                  // v8.0
                  BOOL    fSendMouseCR;
                  BOOL    fMouseHorzMove;
                  BOOL    fMouseVertMove;
                  char    Proxy[128];
                  char    Autorun[512+1];

                  // v8.4
                  BOOL    fPrintToClipboard;

                  struct  {
                            // v9.0
                            UINT    fSshFileConv            : 1;
                            UINT    fSendSshKeyFile         : 1;

                            // v10.2
                            UINT    extBold                 : 5;
                            UINT    extUnderline            : 5;
                            UINT    extBlink                : 5;

                            // v11.0
                            UINT    fEmbeddedKeyfile        : 1;
                            UINT    fTrySSH1                : 1;

                            // v12
                            UINT    fStoreCompressed        : 1;
                            UINT    fUTF8                   : 1;
                            UINT    fSuppressBlink          : 1;
                            UINT    fSuppressUnderline      : 1;

                            // v14
                            UINT    fInitExtKeys1           : 1;

                            // v14.4
                            UINT    fInitExtKeys2           : 1;

                            // v15
                            UINT    fSessionIsUtf8          : 1;

                            // v20
                            UINT    fNoStatusbar            : 1;
                            UINT    obsolete_fNoAspectRatio : 1;
                            UINT    fAsciiPrinting          : 1;
                            UINT    fRunTabbed              : 1;
                            UINT    fKeepAspectRatio        : 1;

                            UINT    fCtrlVPastes            : 1;
                          }
                          xbool;

                  // v9.0
                  int     profile;

                  // v11
                  WORD    keylen1;
                  WORD    keylen2;
                  char    EmbeddedKeyfile[0x1000];

#pragma pack(1)
#pragma option push -a1
                  /*
                   * Ab hier wird nichts mehr in den Sessionfiles eingebettet!
                   * Der Container wird auf tagSESSION->old_F_Key abgebildet.
                   ***********************************************************/
                  struct {

                    // v14
                    char    NumPad_Key[6][MAX_CBKEY];
                    #define NumPadKey_div     container.NumPad_Key[0]
                    #define NumPadKey_mul     container.NumPad_Key[1]
                    #define NumPadKey_minus   container.NumPad_Key[2]
                    #define NumPadKey_plus    container.NumPad_Key[3]
                    #define NumPadKey_CR      container.NumPad_Key[4]

                    // v14.4
                    #define NumPadKey_dec     container.NumPad_Key[5]

                    // v15
                    BYTE    RightBtnAction;   // RBA_CONTEXTMENU, RBA_PASTE, RBA_DISABLED
                    BYTE    CopyPasteAction;  // CAP_DEFAULT, CAP_LOWER, CAP_UPPER, CAP_TOGGLE
                    BYTE    ChangeCaseAction; // CAP_DEFAULT, CAP_LOWER, CAP_UPPER, CAP_TOGGLE

                    #define RightBtnAction    container.RightBtnAction
                    #define CopyPasteAction   container.CopyPasteAction
                    #define ChangeCaseAction  container.ChangeCaseAction

                    char    unused[ 139 ];
                  }
                  container;

                  char Null;
                }
                tagSESSION;
#pragma option pop
#pragma pack()
