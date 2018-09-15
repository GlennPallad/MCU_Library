# Driver for nRF24L01-Plus
This driver is based on **Enhanced ShockBurst™**, and only provides **simplex transmission**, which means you can only use this driver transmit packets from PTX to PRX, ACK was supported(without ACK payload). 
You can modify this driver to support duplex transmission(with ACK payload), but since it's not guaranteed that PTX can receive ACK payload, this driver was not designed to support ACK payload by default.
