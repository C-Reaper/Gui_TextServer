#include "/home/codeleaded/System/Static/Library/CStr.h"
#include "/home/codeleaded/System/Static/Library/Networking_Event.h"


#define NET_EVENT_MSG  (NET_EVENT_START + 0)

void Net_EventServer_Proc_Connect(Net_EventServer* s,Net_EventClient* c,Net_EventPackage* p){
    printf("[Net_EventServer]: Connect -> %d\n",(uint32_t)Net_Client_Id(&c->client));
}
void Net_EventServer_Proc_Disconnect(Net_EventServer* s,Net_EventClient* c,Net_EventPackage* p){
    printf("[Net_EventServer]: Disconnect -> %d\n",(uint32_t)Net_Client_Id(&c->client));
}
void Net_EventServer_Proc_Msg(Net_EventServer* s,Net_EventClient* c,Net_EventPackage* p){
    printf("[Net_EventServer]: Msg -> %u: '%s' (%u)\n",(uint32_t)Net_Client_Id(&c->client),(char*)p->pack.data,(uint32_t)p->pack.size);
    Net_EventServer_Send(s,p->net_event,p->pack.data,p->pack.size);
}

int main(){
    Net_EventServer s = Net_EventServer_Make(5000,10,(Net_EventHandler[]){
        Net_EventHandler_New(NET_EVENT_CONNECT,    (void(*)(void*,Net_EventClient*,Net_EventPackage*))Net_EventServer_Proc_Connect),
        Net_EventHandler_New(NET_EVENT_DISCONNECT, (void(*)(void*,Net_EventClient*,Net_EventPackage*))Net_EventServer_Proc_Disconnect),
        Net_EventHandler_New(NET_EVENT_MSG,        (void(*)(void*,Net_EventClient*,Net_EventPackage*))Net_EventServer_Proc_Msg),
        Net_EventHandler_Null()
    });

    Net_EventServer_Start_C(&s);
    Net_EventServer_Start_R(&s);
    //Net_EventServer_Wait(&s);

    while(1){
        char buffer[1024];
        int size = scanf("%1023s",buffer);
        
        if(CStr_Cmp(buffer,"exit")){
            break;
        }else{
            Net_EventServer_Send(&s,NET_EVENT_MSG,buffer,CStr_Size(buffer) + 1);
            Thread_Sleep_M(100);
        }
    }

    Net_EventServer_Free(&s);
    return 0;
}