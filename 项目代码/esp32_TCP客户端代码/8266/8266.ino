/*
  ESP8266-NodeMCU作为TcpClient连接到服务器
  修改说明：
  1. 去掉收到消息就关闭TCP连接的逻辑
  2. 添加从服务器收到数据后将数据用串口转发的功能
  3. 添加将从串口接收到的数据转发给TCP服务器的功能
  4. 保持TCP连接持续运行，实现双向数据传输
*/

#include <ESP8266WiFi.h>                        // 本程序使用ESP8266WiFi库

const char* ssid     = "5200";                // 需要连接到的WiFi名
const char* password = "qwer1234";             // 连接的WiFi密码

const char* host = "192.168.244.207";
const uint16_t port = 8266;

WiFiClient tcpclient;                           // WiFiClient对象定义为全局变量，方便在loop中持续使用
bool isConnected = false;                       // 连接状态标志
unsigned long lastReconnectAttempt = 0;         // 上次重连尝试时间
const unsigned long RECONNECT_INTERVAL = 5000;  // 重连间隔(毫秒)

// 串口缓冲区相关变量
String serialBuffer = "";                       // 存储从串口接收的数据
const unsigned long SERIAL_TIMEOUT = 50;        // 串口数据接收超时时间(毫秒)
unsigned long lastSerialDataTime = 0;           // 上次收到串口数据的时间
bool serialDataReady = false;                   // 串口数据准备好标志

void setup() {
  Serial.begin(115200);                         // 初始化串口通讯波特率为115200
  Serial.setTimeout(10);                        // 设置串口读取超时时间

  WiFi.mode(WIFI_STA);                          // 设置Wifi工作模式为STA,默认为AP+STA模式
  WiFi.begin(ssid, password);                   // 通过wifi名和密码连接到Wifi
  Serial.print("\r\nConnecting to ");           // 串口监视器输出网络连接信息
  Serial.print(ssid); Serial.println(" ...");   // 显示NodeMCU正在尝试WiFi连接
  
  int i = 0;                                    // 检查WiFi是否连接成功
  while (WiFi.status() != WL_CONNECTED)         // WiFi.status()函数的返回值是由NodeMCU的WiFi连接状态所决定的。 
  {                                             // 如果WiFi连接成功则返回值为WL_CONNECTED
    delay(1000);                                // 此处通过While循环让NodeMCU每隔一秒钟检查一次WiFi.status()函数返回值
    Serial.print("waiting for ");                          
    Serial.print(i++); Serial.println("s...");       
  }                                             
                                               
  Serial.println("");                           // WiFi连接成功后
  Serial.println("WiFi connected!");            // NodeMCU将通过串口监视器输出"连接成功"信息。
  Serial.print("IP address: ");                 // 同时还将输出NodeMCU的IP地址。这一功能是通过调用
  Serial.println(WiFi.localIP());               // WiFi.localIP()函数来实现的。该函数的返回值即NodeMCU的IP地址。
  
  pinMode(2, OUTPUT);                           // 初始化板载LED
  
  // 连接TCP服务器
  connectToServer();
}

void connectToServer() {
  /* 建立TCP连接 */
  Serial.print("Connecting to TCP server ");
  Serial.print(host); 
  Serial.print(":");
  Serial.println(port);
  
  if (tcpclient.connect(host, port)) {
    Serial.println("Connected to TCP server successfully!");
    isConnected = true;
    
    // 发送初始连接消息
    tcpclient.println("ESP8266 connected, ready for data exchange.");
    Serial.println("Initial message sent to server.");
    
    digitalWrite(2, HIGH);  // 连接成功时点亮LED
  } else {
    Serial.println("TCP connection failed!");
    isConnected = false;
    digitalWrite(2, LOW);   // 连接失败时关闭LED
  }
}

void handleSerialData() {
  // 检查串口是否有数据
  while (Serial.available() > 0) {
    char ch = Serial.read();
    
    // 处理特殊字符（如换行符）
    if (ch == '\n' || ch == '\r') {
      if (serialBuffer.length() > 0) {
        serialDataReady = true;
        lastSerialDataTime = millis();  // 更新最后接收时间
      }
    } else {
      serialBuffer += ch;  // 添加到缓冲区
      lastSerialDataTime = millis();  // 更新最后接收时间
    }
  }
  
  // 检查超时，如果一段时间没有收到新数据，则认为数据包已完整
  if (!serialDataReady && serialBuffer.length() > 0 && 
      (millis() - lastSerialDataTime) > SERIAL_TIMEOUT) {
    serialDataReady = true;
  }
  
  // 如果串口数据准备好且有TCP连接，发送到服务器
  if (serialDataReady && serialBuffer.length() > 0) {
    if (isConnected && tcpclient.connected()) {
      // 发送数据到TCP服务器
      tcpclient.print(serialBuffer);
      Serial.print("[Sent to server] ");
      Serial.println(serialBuffer);
      
      // LED闪烁指示数据发送
      digitalWrite(2, LOW);
      delay(10);
      digitalWrite(2, HIGH);
    } else {
      Serial.print("[Not connected, cannot send] ");
      Serial.println(serialBuffer);
    }
    
    // 清空缓冲区
    serialBuffer = "";
    serialDataReady = false;
  }
}

void handleTcpData() {
  // 检查TCP服务器是否有数据
  if (tcpclient.available() > 0) {
    
    // 读取并转发所有可用数据到串口
    while (tcpclient.available()) {
      char ch = static_cast<char>(tcpclient.read());
      Serial.print(ch);  // 将接收到的数据通过串口转发
    }
    Serial.println();  // 换行，使输出更清晰
    
    // LED闪烁指示数据接收
    digitalWrite(2, LOW);
    delay(50);
    digitalWrite(2, HIGH);
  }
}

void loop() {
  unsigned long currentTime = millis();
  
  // 检查WiFi连接状态
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, attempting to reconnect...");
    WiFi.reconnect();
    isConnected = false;
    delay(2000);
    return;
  }
  
  // 检查TCP连接状态，如果未连接则尝试重连
  if (!isConnected || !tcpclient.connected()) {
    if (currentTime - lastReconnectAttempt > RECONNECT_INTERVAL) {
      Serial.println("TCP connection lost, attempting to reconnect...");
      connectToServer();
      lastReconnectAttempt = currentTime;
    }
    delay(100);
    return;
  }
  
  // 处理从TCP服务器接收的数据
  handleTcpData();
  
  // 处理从串口接收的数据
  handleSerialData();
  
  // 短暂延迟，避免过于频繁的循环
  delay(10);
}

// 辅助函数：检查连接状态
void printConnectionStatus() {
  Serial.println("\n--- Connection Status ---");
  Serial.print("WiFi Status: ");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Disconnected");
  }
  
  Serial.print("TCP Connection: ");
  if (tcpclient.connected()) {
    Serial.println("Connected");
    Serial.print("Remote: ");
    Serial.print(host);
    Serial.print(":");
    Serial.println(port);
  } else {
    Serial.println("Disconnected");
  }
  Serial.println("-------------------------\n");
}
