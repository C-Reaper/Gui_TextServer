#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/Networking.h"
#include "/home/codeleaded/System/Static/Library/CStr.h"
#include "/home/codeleaded/System/Static/Library/Networking.h"


#define SIGNAL_MSG  (SIGNAL_START + 0)
Server s;
TextBox tb;

void Server_Proc_Connect(Vector* signalhandlers,Client* c,void* data,int size){
    printf("Server_Connect: %d\n",c->sockfd);
}
void Server_Proc_Disconnect(Vector* signalhandlers,Client* c,void* data,int size){
    printf("Server_Disconnect: %d\n",c->sockfd);
}
void Server_Proc_Msg(Vector* signalhandlers,Client* c,void* data,int size){
    printf("Server_Msg: %d -> '%s' (%d)\n",c->sockfd,(char*)data,size);
}

void Setup(AlxWindow* w){
	s = Server_Make(5000,10,(SignalHandler[]){
        SignalHandler_New(SIGNAL_CONNECT,(void (*)(void*,void*,void*,int))Server_Proc_Connect),
        SignalHandler_New(SIGNAL_DISCONNECT,(void (*)(void*,void*,void*,int))Server_Proc_Disconnect),
        SignalHandler_New(SIGNAL_MSG,(void (*)(void*,void*,void*,int))Server_Proc_Msg),
        SignalHandler_Null()
    });

    Server_Start(&s);

	tb = TextBox_New(
		Input_New(100,1),
		(Rect){ 0.0f,0.0f,w->Width,w->Height / 15 },
		ALXFONT_PATHS_BLOCKY,
		16,
		16,
		WHITE
	);
}
void Update(AlxWindow* w){
	TextBox_Update(&tb,window.Strokes,GetMouse());

	if(Stroke(ALX_KEY_ENTER).PRESSED){
		CStr out = String_CStr(&tb.In.Buffer);

		if(CStr_Cmp(out,"exit"))
			w->Running = 0;

        Server_Signal_Send(&s,SIGNAL_MSG,out,tb.In.Buffer.size + 1);
	}

	Clear(BLACK);

	TextBox_Render(WINDOW_STD_ARGS,&tb);
}
void Delete(AlxWindow* w){
    TextBox_Free(&tb);
    Server_Free(&s);
}

int main(){
    if(Create("",2200,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}