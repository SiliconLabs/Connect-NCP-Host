#ifndef __CALLBACK_QUEUE_H__
#define __CALLBACK_QUEUE_H__

void sli_init_callback_queue();
void sli_connect_ncp_append_callback_command(uint8_t *callback_command, uint16_t command_length);

#endif
