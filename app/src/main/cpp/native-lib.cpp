#include <jni.h>
#include <string>
#include <stdlib.h>
#include "android/log.h"


#define ADDRESS     "tcp://192.168.0.157:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "test"
#define PAYLOAD     "Hello World!"
#define QOS         0
#define TIMEOUT     10000L
#define TAG "CONN_TCP"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
extern "C" {
#include "include/MQTTClient.h"
}





extern "C" {

volatile MQTTClient_deliveryToken deliveredtoken;
void connlost(void *context, char *cause) {
    LOGE("\nConnection lost\n");
    LOGE("     cause: %s\n", cause);
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    int i;
    char *payloadptr;
    LOGE("Message arrived\n");
    LOGE("     topic: %s\n", topicName);
    LOGE("   message: ");
    payloadptr = (char *) message->payload;
    for (i = 0; i < message->payloadlen; i++) {
        putchar(*payloadptr++);
    }
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
void delivered(void *context, MQTTClient_deliveryToken dt) {
    LOGE("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}
JNIEXPORT void JNICALL
Java_com_ryanrq_mosquittonandroid_MainActivity_startMqtt(JNIEnv *env, jobject instance) {

    // TODO
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    char ch;
    MQTTClient_create(&client, ADDRESS, CLIENTID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
    //conn_opts.keepAliveInterval = 20;
    //conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    //½¨Á¢Á¬½Ó
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        LOGE("Failed to connect, return code %d\n", rc);
        // exit(EXIT_FAILURE);
    }
    LOGE("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
                 "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    //¶©ÔÄÖ÷Ìâ
    MQTTClient_subscribe(client, TOPIC, QOS);
    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    pubmsg.payload = PAYLOAD;
    pubmsg.payloadlen = strlen(PAYLOAD);
    MQTTClient_deliveryToken token;
    while (true) {
//        if (ch != 'Q' && ch != 'q'){
//            MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
//        }
//        else
//        {
//            break;
//        }
    }
    //Ïú»Ù
    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);
}

}