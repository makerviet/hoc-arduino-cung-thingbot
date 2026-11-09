/*
 * Chuong 1 - Xin chao ThingBot
 * Chuong trinh dau tien: in chu ra Serial Monitor va dem so giay.
 * Mo Tools > Serial Monitor, chon 115200 baud.
 */

int counter = 0;  // bien dem

// setup() chay MOT LAN khi cap nguon hoac nhan RESET
void setup() {
  Serial.begin(115200);  // mo cong noi tiep toc do 115200 bit/giay
  delay(1500);           // doi may tinh ket noi cong USB
  Serial.println("Xin chao, minh la ThingBot!");
  Serial.print("Chip: ");
  Serial.println(ESP.getChipModel());
}

// loop() chay LAP LAI mai mai
void loop() {
  counter = counter + 1;
  Serial.print("Da chay duoc ");
  Serial.print(counter);
  Serial.println(" giay");
  delay(1000);  // nghi 1000 ms = 1 giay
}
