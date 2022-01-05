/**
 *  Copyright 2013 Silicon Laboratories, Inc.
 *  http://www.silabs.com
 *
 *  @file sRange.h
 *  @author Szilveszter Papp
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 *  This software must be used in accordance with the
 *  End User License Agreement.
 */

#ifndef SRANGE_H_
#define SRANGE_H_

#define APP_FW_VERSION  "FW ver. 2.00"

typedef struct {
  unsigned char  RadioMode;
  unsigned char  DestID;
  unsigned char  SrcID;
  unsigned char  UARTlog;
  unsigned char  RSSIMode;
  unsigned char  PktPayload;
  unsigned char  ConfigNr;
  unsigned char  PAIndex;
  unsigned char  DemoMode;
  unsigned char  TRXSlave;
  unsigned char  TRXMaster;
  unsigned char  MAWindowSize;
  unsigned char  ReTransmit;
  unsigned int PktNum;
  unsigned int PktCnt;
  unsigned int ActPktCounter;
  unsigned int PktCntOffset;
  unsigned int MAInt;
  unsigned int PERInt;
  unsigned int PacketValidCount;
  unsigned int PacketSentCount;
  unsigned char RSSI;
} sRange_t;

typedef enum {
  SRANGE_RSSI_ICON,
  SRANGE_RSSI_NUMBER,
  SRANGE_RSSI_DBM,
};

typedef enum {
  SRANGE_UART_LOG_OFF,
  SRANGE_UART_LOG_ON,
};

typedef enum {
  SRANGE_DEMO_STOP,
  SRANGE_DEMO_IDLE,
  SRANGE_DEMO_RUN,
};


/** Global Variables */
extern SI_SEGMENT_VARIABLE(sRange, sRange_t, SI_SEG_XDATA);

#endif /* SRANGE_H_ */
