
### [필수 기능]
  전체 구현 
### [도전 기능]
  실시간 타이머 위젯
  입력 차단
  턴 전환 시스템
  채팅 로그

### 코드 설명
  BaseGameMode        # 게임 로직 (숫자 생성, 판정, 턴 관리, 승패 판정)
  BaseGameState       # 게임 상태 리플리케이션 (남은 시간, 현재 턴 플레이어)                                                                
  BasePlayerState     # 플레이어 상태 (이름, 추리 횟수, 최대 횟수)                                                                             
  BasePlayerController # 클라이언트-서버 RPC 통신, UI 위젯 관리                                                                                
  ChatInput           # 채팅 입력 위젯 (EditableTextBox)                                                                                       
  ChatLogWidget       # 채팅 로그 위젯 (ScrollBox)                                                                                             
  NotificationWidget  # 알림 위젯 (접속, 승리 메시지 등)                                                                                       
  TimerWidget         # 턴 타이머 위젯   

## RPC
  | 방향 | RPC | 용도 |                                                                                                                            
  |------|-----|------|                                                                                                                            
  | Client → Server | `ServerRPCPrintChatMessageString` | 채팅/추리 입력 전송 |                                                                    
  | Server → Client | `ClientRPCPrintChatMessageString` | 채팅 메시지 브로드캐스트 |                                                               
  | Server → Client | `ClientRPCClearChatLog` | 게임 리셋 시 채팅 로그 초기화 |                                                                    
  | Server → All | `MulticastRPCBroadcastLoginMessage` | 플레이어 접속 알림 |    

## Property Replication
`RemainingTime`, `CurrentTurnPlayerName`, `NotificationText`, `PlayerNameString`, `CurrentGuessCount`, `MaxGuessCount`  
