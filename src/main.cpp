/*
 * LoRa E220
 * Get configuration.
 * You must uncommend the correct constructor.
 *
 * by Renzo Mischianti <https://www.mischianti.org>
 *
 * https://www.mischianti.org
 *
 */

#include "Arduino.h"
#include "LoRa_E220.h"

#define TX_PIN 17
#define RX_PIN 16
#define AUX_PIN 255
#define M0_PIN 23
#define M1_PIN 22

#define ADDH_ADDRESS 0x00
#define ADDL_ADDRESS 0x03
#define TRANSMISSION_CHANNEL 23

LoRa_E220 e220ttl(&Serial2, AUX_PIN, M0_PIN, M1_PIN, UART_BPS_RATE_9600);

void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);

void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);

void setup() {
  
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
	Serial.begin(9600);
	while(!Serial){};
	Serial.println();

	e220ttl.begin();

	ResponseStructContainer c;
	c = e220ttl.getConfiguration();

	Configuration configuration = *(Configuration*) c.data;
	Serial.println(c.status.getResponseDescription());
	Serial.println(c.status.code);

  configuration.ADDL = 0x03;
	configuration.ADDH = 0x00;

	configuration.CHAN = 23;

	configuration.SPED.uartBaudRate = UART_BPS_9600;
	configuration.SPED.airDataRate = AIR_DATA_RATE_010_24;
	configuration.SPED.uartParity = MODE_00_8N1;

	configuration.OPTION.subPacketSetting = SPS_200_00;
	configuration.OPTION.RSSIAmbientNoise = RSSI_AMBIENT_NOISE_DISABLED;
	configuration.OPTION.transmissionPower = POWER_22;

	configuration.TRANSMISSION_MODE.enableRSSI = RSSI_DISABLED;
	configuration.TRANSMISSION_MODE.fixedTransmission = FT_FIXED_TRANSMISSION;
	configuration.TRANSMISSION_MODE.enableLBT = LBT_DISABLED;
	configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_011;

  ResponseStatus rs = e220ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
	Serial.println(rs.getResponseDescription());
	Serial.println(rs.code);

	c = e220ttl.getConfiguration();

	configuration = *(Configuration*) c.data;
	Serial.println(c.status.getResponseDescription());
	Serial.println(c.status.code);

	printParameters(configuration);

	c.close();

}

void loop() {

  // Serial.println("Hi, I'm going to send message!");

	// // Send message
	// ResponseStatus rs = e220ttl.sendFixedMessage(ADDH_ADDRESS, ADDL_ADDRESS, TRANSMISSION_CHANNEL, "Mensagem teste para envio!");
	// // Check If there is some problem of succesfully send
	// Serial.println(rs.getResponseDescription());

    if (e220ttl.available()>1) {
      Serial.println("Message received!");

      // read the String message
      #ifdef ENABLE_RSSI
      ResponseContainer rc = e220ttl.receiveMessageRSSI();
      #else
      ResponseContainer rc = e220ttl.receiveMessage();
      #endif
      // Is something goes wrong print error
      if (rc.status.code!=1){
          Serial.println(rc.status.getResponseDescription());
        }else{
          // Print the data received
          Serial.println(rc.status.getResponseDescription());
          Serial.println(rc.data);
        #ifdef ENABLE_RSSI
          Serial.print("RSSI: "); Serial.println(rc.rssi, DEC);
      #endif
    }
  }

  delay(2000);

}

void printParameters(struct Configuration configuration) {
	Serial.println("----------------------------------------");

	Serial.print(F("HEAD : "));  Serial.print(configuration.COMMAND, HEX);Serial.print(" ");Serial.print(configuration.STARTING_ADDRESS, HEX);Serial.print(" ");Serial.println(configuration.LENGHT, HEX);
	Serial.println(F(" "));
	Serial.print(F("AddH : "));  Serial.println(configuration.ADDH, HEX);
	Serial.print(F("AddL : "));  Serial.println(configuration.ADDL, HEX);
	Serial.println(F(" "));
	Serial.print(F("Chan : "));  Serial.print(configuration.CHAN, DEC); Serial.print(" -> "); Serial.println(configuration.getChannelDescription());
	Serial.println(F(" "));
	Serial.print(F("SpeedParityBit     : "));  Serial.print(configuration.SPED.uartParity, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTParityDescription());
	Serial.print(F("SpeedUARTDatte     : "));  Serial.print(configuration.SPED.uartBaudRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTBaudRateDescription());
	Serial.print(F("SpeedAirDataRate   : "));  Serial.print(configuration.SPED.airDataRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getAirDataRateDescription());
	Serial.println(F(" "));
	Serial.print(F("OptionSubPacketSett: "));  Serial.print(configuration.OPTION.subPacketSetting, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getSubPacketSetting());
	Serial.print(F("OptionTranPower    : "));  Serial.print(configuration.OPTION.transmissionPower, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getTransmissionPowerDescription());
	Serial.print(F("OptionRSSIAmbientNo: "));  Serial.print(configuration.OPTION.RSSIAmbientNoise, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getRSSIAmbientNoiseEnable());
	Serial.println(F(" "));
	Serial.print(F("TransModeWORPeriod : "));  Serial.print(configuration.TRANSMISSION_MODE.WORPeriod, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getWORPeriodByParamsDescription());
	Serial.print(F("TransModeEnableLBT : "));  Serial.print(configuration.TRANSMISSION_MODE.enableLBT, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getLBTEnableByteDescription());
	Serial.print(F("TransModeEnableRSSI: "));  Serial.print(configuration.TRANSMISSION_MODE.enableRSSI, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getRSSIEnableByteDescription());
	Serial.print(F("TransModeFixedTrans: "));  Serial.print(configuration.TRANSMISSION_MODE.fixedTransmission, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getFixedTransmissionDescription());


	Serial.println("----------------------------------------");
}
void printModuleInformation(struct ModuleInformation moduleInformation) {
	Serial.println("----------------------------------------");
	Serial.print(F("HEAD: "));  Serial.print(moduleInformation.COMMAND, HEX);Serial.print(" ");Serial.print(moduleInformation.STARTING_ADDRESS, HEX);Serial.print(" ");Serial.println(moduleInformation.LENGHT, DEC);

	Serial.print(F("Model no.: "));  Serial.println(moduleInformation.model, HEX);
	Serial.print(F("Version  : "));  Serial.println(moduleInformation.version, HEX);
	Serial.print(F("Features : "));  Serial.println(moduleInformation.features, HEX);
	Serial.println("----------------------------------------");
}