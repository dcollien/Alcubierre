#ifndef ALC_NET_NET_H_
#define ALC_NET_NET_H_

#define NET_SERVER_ADDRESS 0
#define NET_PLAYER_ADDRESS 1

typedef int net_address_t;

/**
 * @brief Sends a message in the required protocol over the network to the
 *  "virtual" address that we want to communicate with.
 * @param net_address_t The place you want to send this message.
 * @param protocol the protocol you have constructed.
 * @return True if the message sent else 0.
 */
int send_net(net_address_t, protocol_t protocol);

#endif /* ALC_NET_NET_H_ */
