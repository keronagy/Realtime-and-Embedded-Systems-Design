uint32_t PortF_Input(void){     
  return (GPIO_PORTF_DATA_R&0x11);  // read PF4,PF0 inputs
}

void PortF_Output(uint32_t data){ // write PF3-PF1 outputs
  GPIO_PORTF_DATA_R = data;      
}

