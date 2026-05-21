#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/spi/spi.h"

namespace esphome {
namespace xl4432_spi_sensor {
class Xl4432SPISensor;
}  // namespace xl4432_spi_sensor
}  // namespace esphome

#define FIFO_FULL    0x40
#define VALID_PACKET 0x02
#define PACKET_SIZE  0x15

enum PacketStatus {
  PKT_VALID,
  PKT_VALID_TWO_PKT,
  PKT_CORRECTED_1,
  PKT_CORRECTED_2,
  PKT_CORRECTED_3,
  PKT_INVALID,
  PKT_ID_MISMATCH,
  PKT_NON_STANDARD,
  PKT_SNIFF,
};

class Xl4432 {
  public:
	char  METER_ID[3];
	volatile int nIRQState;
	uint8_t packet[PACKET_SIZE];
	char  output[(PACKET_SIZE * 2) + 1];
	float meterMeasurment;
	float lastMeterMeasurment;
	bool  packetReady;
	bool  useIdAsSync;
	bool  packetSniff;
	void  spiDisableReciver();
	void  spiEnableReciver();
	void  checkForNewPacket();
	void  initXl4432Registers();
	Xl4432(char id[3], bool use_id_as_sync = false);
	void set_spi_client(esphome::xl4432_spi_sensor::Xl4432SPISensor *client) { spi_device_ = client; }
	uint64_t expectedScramble();
	uint64_t deriveConstant();
	PacketStatus validatePacket();
	uint64_t storedConstant;
	bool     hasStoredConstant;

	float extractMeterReading();
	void  binToHexString();

  private:
	void  readPacketFromFifo();
	void  spiInitRadio();
	void  spiXl4432Fifo();
	void  spiReadPacketFromFifo(uint8_t length);
	uint8_t spiReadRegister(uint8_t addr);
	void  spiWriteRegister(uint8_t addr, uint8_t data);
	esphome::xl4432_spi_sensor::Xl4432SPISensor *spi_device_{nullptr};
};
