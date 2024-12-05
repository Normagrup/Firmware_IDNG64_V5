typedef  unsigned char uint8_t;			
		
		void spiIINCHIP_CSoff_m25p (void)
		{
			boardSPIM25PSSEnable();
		}

		void spiIINCHIP_CSon_m25p (void)
		{
			boardSPIM25PSSDisable();
		}
		
		void spiIINCHIP_CSoff_w5100 (void)
		{
			boardSPIW5100SSEnable();
		}

		void spiIINCHIP_CSon_w5100 (void)
		{
			boardSPIW5100SSDisable();
		}
		
		
		
		void spiIINCHIP_SpiSendData(unsigned char data)
		{
			boardSPI2Write(data);
		}
		
		
		
		
		
		uint8_t spiIINCHIP_SpiRecvData(void)
		{
			return boardSPI2Read();
		}
