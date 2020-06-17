/// @file device.cpp
/// @brief  Contains the tests for the functions and structures for the devices.

#include "gtest/gtest.h"

extern "C" {
#include "device.h"
}

// /*
//  * Test check send message
//  */
// TEST(check_send_messages, ok) {
//   shm_ack = (Acknowledgment *)malloc(sizeof(Acknowledgment) * ACK_SIZE *
//                                      DEVICE_NUMBER);
//
//   pid_t pid = 1234;
//   node_message *node = (node_message *)malloc(sizeof(node_message));
//   node->value.max_distance = 20;
//   strcpy(node->value.message, "Test");
//   node->value.message_id = 1;
//   node->value.pid_sender = 0;
//   node->value.pid_receiver = pid;
//   list_message *list = create_list_message(node, node, 0);
//
//   list_message *s_list = check_send_messages(pid, 0, {0, 0}, list);
// }

