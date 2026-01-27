#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/Networking.h"
#include "/home/codeleaded/System/Static/Library/CStr.h"
#include "/home/codeleaded/System/Static/Library/Networking.h"


#define SIGNAL_MSG  (SIGNAL_START + 0)
Client c;
TextBox text;
TextBox tb;
TextBox nametb;
String buffer;

void Client_Proc_Connect(void* parent,Signal sig,Client* c,void* data,int size){
    String_Appendf(&text.In.Buffer,"[%d]: Connect!\n",c->sockfd);
    Client_Signal_Send(c,SIGNAL_CONNECT,NULL,0);
}
void Client_Proc_Disconnect(void* parent,Signal sig,Client* c,void* data,int size){
    String_Appendf(&text.In.Buffer,"[%d]: Disconnect!\n",c->sockfd);
    Client_Signal_Send(c,SIGNAL_DISCONNECT,NULL,0);
}
void Client_Proc_Msg(void* parent,Signal sig,Client* c,void* data,int size){
    String_Appendf(&text.In.Buffer,"%s\n",(char*)data);
}

void Setup(AlxWindow* w){
	c = Client_Make("5000","192.168.2.99",(SignalHandler[]){
        SignalHandler_New(SIGNAL_CONNECT,(void (*)(void*,Signal,void*,void*,int))Client_Proc_Connect),
        SignalHandler_New(SIGNAL_DISCONNECT,(void (*)(void*,Signal,void*,void*,int))Client_Proc_Disconnect),
        SignalHandler_New(SIGNAL_MSG,(void (*)(void*,Signal,void*,void*,int))Client_Proc_Msg),
        SignalHandler_Null()
    });

	text = TextBox_New(
		Input_New(30,INPUT_MAXLENGTH),
		(Rect){ 0.0f,0.0f,w->Width,w->Height * 9 / 10 },
		ALXFONT_PATHS_BLOCKY,
		32,
		32,
		GRAY
	);
    tb = TextBox_New(
		Input_New(100,1),
		(Rect){ 0.0f,w->Height * 9 / 10,w->Width * 7 / 10,w->Height / 10 },
		ALXFONT_PATHS_BLOCKY,
		32,
		32,
		BLACK
	);
    nametb = TextBox_New(
		Input_New(16,1),
		(Rect){ w->Width * 7 / 10,w->Height * 9 / 10,w->Width * 3 / 10,w->Height / 10 },
		ALXFONT_PATHS_BLOCKY,
		32,
		32,
		DARK_GRAY
	);
}
void Update(AlxWindow* w){
	//TextBox_Update(&text,window.Strokes,GetMouse());
	TextBox_Update(&tb,window.Strokes,GetMouse());
	TextBox_Update(&nametb,window.Strokes,GetMouse());

    text.r = (Rect){ 0.0f,0.0f,w->Width,w->Height * 9 / 10 };
    tb.r = (Rect){ 0.0f,w->Height * 9 / 10,w->Width * 7 / 10,w->Height / 10 };
    nametb.r = (Rect){ w->Width * 7 / 10,w->Height * 9 / 10,w->Width * 3 / 10,w->Height / 10 };

    
    Client_Update(&c);
    Client_DoAll(&c,NULL);

	if(Stroke(ALX_KEY_ENTER).PRESSED){
        String_Add(&tb.In.Buffer,"]: ",0);
        String_AddString(&tb.In.Buffer,&nametb.In.Buffer,0);
        String_Add(&tb.In.Buffer,"[",0);
		CStr out = String_CStr(&tb.In.Buffer);

        Client_Signal_Send(&c,SIGNAL_MSG,out,tb.In.Buffer.size + 1);
        CStr_Free(&out);

        String_Clear(&tb.In.Buffer);
	}

	Clear(WHITE);

    TextBox_Render(WINDOW_STD_ARGS,&text);
	TextBox_Render(WINDOW_STD_ARGS,&tb);
	TextBox_Render(WINDOW_STD_ARGS,&nametb);
}
void Delete(AlxWindow* w){
    TextBox_Free(&text);
    TextBox_Free(&tb);
    TextBox_Free(&nametb);
    Client_Free(&c);
}

int main(){
    if(Create("Text:Client",1000,1000,1,1,Setup,Update,Delete))
        Start();
    return 0;
}