struct Smart_table_lamp: Service::LightBulb 
{
  int control_pin;
  SpanCharacteristic *lampPower;  

  Smart_table_lamp(int control_pin): Service::LightBulb()
  {
    lampPower = new Characteristic::On();
    this->control_pin = control_pin;
    pinMode(control_pin,OUTPUT);
  }

  boolean update()
  {
    digitalWrite(control_pin,lampPower->getNewVal());
    return true;
  }
};