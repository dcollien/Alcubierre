#ifndef ALC_PROTOCOL_H_
#define ALC_PROTOCOL_H_

struct message {
    size_t type_t;
    size_t len;
    char *data;
};

#endif /* ALC_PROTOCOL_H_ */
