#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/Networking.h"
#include "/home/codeleaded/System/Static/Library/CStr.h"
#include "/home/codeleaded/System/Static/Library/Networking.h"


#define SIGNAL_MSG  (SIGNAL_START + 0)
Client c;
TextBox tb;

void Client_Proc_Connect(Vector* signalhandlers,Client* c,void* data,int size){
    printf("Client_Connect: %d\n",c->sockfd);
    Client_Signal_Send(c,SIGNAL_CONNECT,NULL,0);
}
void Client_Proc_Disconnect(Vector* signalhandlers,Client* c,void* data,int size){
    printf("Client_Disconnect: %d\n",c->sockfd);
    Client_Signal_Send(c,SIGNAL_DISCONNECT,NULL,0);
}
void Client_Proc_Msg(Vector* signalhandlers,Client* c,void* data,int size){
    printf("Client_Msg: %d -> '%s' (%d)\n",c->sockfd,(char*)data,size);
}

void Setup(AlxWindow* w){
	c = Client_Make("5000","192.168.2.99",(SignalHandler[]){
        SignalHandler_New(SIGNAL_CONNECT,(void (*)(void*,void*,void*,int))Client_Proc_Connect),
        SignalHandler_New(SIGNAL_DISCONNECT,(void (*)(void*,void*,void*,int))Client_Proc_Disconnect),
        SignalHandler_New(SIGNAL_MSG,(void (*)(void*,void*,void*,int))Client_Proc_Msg),
        SignalHandler_Null()
    });

	tb = TextBox_New(
		Input_New(100,1),
		(Rect){ 0.0f,w->Height * 9 / 10,w->Width,w->Height / 10 },
		ALXFONT_PATHS_BLOCKY,
		32,
		32,
		BLACK
	);
}
void Update(AlxWindow* w){
	TextBox_Update(&tb,window.Strokes,GetMouse());
        
    Client_Update(&c);
    Client_DoAll(&c,NULL);

	if(Stroke(ALX_KEY_ENTER).PRESSED){
		CStr out = String_CStr(&tb.In.Buffer);

        Client_Signal_Send(&c,SIGNAL_MSG,out,tb.In.Buffer.size + 1);
        CStr_Free(&out);

        String_Clear(&tb.In.Buffer);
	}

	Clear(WHITE);

	TextBox_Render(WINDOW_STD_ARGS,&tb);
}
void Delete(AlxWindow* w){
    TextBox_Free(&tb);
    Client_Free(&c);
}

int main(){
    if(Create("Text:Client",1000,1000,1,1,Setup,Update,Delete))
        Start();
    return 0;
}