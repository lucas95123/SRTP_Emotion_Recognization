int pinLed = 13;//定义连接LED的数字口，当允许通过串口发送数据时，点亮LED，否则关闭LED
boolean sendFlag = false;//指示是否允许通过串口发送数据
boolean readCompleted = false;//指示是否完成读取串口数据
String serialString = "";//串口数据缓存字符串
 
//Author：Alex Leo, Email:conexpress@qq.com, Blog:http://conexpress.cnblogs.com/
//参考:http://arduino.cc/en/Reference/Serial
void setup()
{
  pinMode(pinLed,OUTPUT);
  Serial.begin(9600);
  serialString.reserve(200);//初始化字符串
}
 
void loop()
{
  int lightValue = analogRead(A0);//从A0口读取光线传感器的值
  if(readCompleted)//判断串口是否接收到数据并完成读取
  {
    Serial.print("read value:");
    Serial.println(serialString);//将读取到的信息发送给电脑
    if(serialString == "serial start")//当读取到的信息是"serial start"时，设置发送标志设置为true
    {
      sendFlag = true;
    }
    else if(serialString == "serial stop")//当读取到的信息是"serial stop"时，设置发送标志设置为false
    {
      sendFlag = false;
    }
    serialString = "";
    readCompleted = false;
  }
 
  if(sendFlag)//如果允许通过串口发送数据，则点亮LED并发送数据，否则关闭LED
  {
    digitalWrite(pinLed, HIGH);
    Serial.print("light value:");
    Serial.println(lightValue);
  }
  else
  {
    digitalWrite(pinLed, LOW);
  }
  delay(1000);//延时1000ms
}
 
void serialEvent()//串口事件处理方法，参考:http://arduino.cc/en/Tutorial/SerialEvent
{
  while(Serial.available())//参考://arduino.cc/en/Serial/Available
  {
    char inChar = (char)Serial.read();
    if(inChar != '\n')//以换行符作为读取结束标志
    {
      serialString += inChar;
    }
    else
    {
      readCompleted = true;
    }
  }
}