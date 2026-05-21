import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import spi, sensor
from esphome.const import CONF_ID, ICON_EMPTY, UNIT_EMPTY
from esphome import pins

DEPENDENCIES = ['spi']

xl4432_spi_sensor_ns = cg.esphome_ns.namespace('xl4432_spi_sensor')
Xl4432SPISensor = xl4432_spi_sensor_ns.class_('Xl4432SPISensor', cg.PollingComponent,
                                  spi.SPIDevice)

CONFIG_SCHEMA = sensor.sensor_schema(unit_of_measurement=UNIT_EMPTY,icon=ICON_EMPTY, accuracy_decimals=1).extend({
    cv.GenerateID(): cv.declare_id(Xl4432SPISensor),
    cv.Required('meter_id'): cv.string,
    cv.Optional('irq_pin', default=5): pins.internal_gpio_input_pullup_pin_schema,
    cv.Optional('packet_sniff', default=False): cv.boolean,
    cv.Optional('tcp_server', default=False): cv.boolean,
}).extend(cv.polling_component_schema('5s')).extend(spi.spi_device_schema())


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)
    yield spi.register_spi_device(var, config)

    # Set the meter ID
    meter_id_str = config['meter_id']
    cg.add(var.set_meter_id(meter_id_str))
    irq_pin = yield cg.gpio_pin_expression(config['irq_pin'])
    cg.add(var.set_irq_pin(irq_pin))
    cg.add(var.set_packet_sniff(config['packet_sniff']))
    cg.add(var.set_tcp_server(config['tcp_server']))
    