#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/Networking_Event.h"


#define NET_EVENT_MSG  (NET_EVENT_START + 0)

Net_EventClient c;
TextBox text;
TextBox tb;
TextBox nametb;
String buffer;

void Net_EventClient_Proc_Connect(void* parent,Net_EventClient* c,Net_EventPackage* p){
    String_Appendf(&text.In.Buffer,"[%u]: Connect!\n",(uint32_t)Net_Client_Id(&c->client));
    Net_EventClient_Send(c,NET_EVENT_CONNECT,NULL,0);
}
void Net_EventClient_Proc_Disconnect(void* parent,Net_EventClient* c,Net_EventPackage* p){
    String_Appendf(&text.In.Buffer,"[%u]: Disconnect!\n",(uint32_t)Net_Client_Id(&c->client));
    Net_EventClient_Send(c,NET_EVENT_DISCONNECT,NULL,0);
}
void Net_EventClient_Proc_Msg(void* parent,Net_EventClient* c,Net_EventPackage* p){
	Vector_PushCount(&text.In.Buffer,p->pack.data,p->pack.size);
	String_AppendChar(&text.In.Buffer,'\n');
}

void Setup(AlxWindow* w){
	c = Net_EventClient_Make("5000","192.168.2.99",(Net_EventHandler[]){
        Net_EventHandler_New(NET_EVENT_CONNECT,     Net_EventClient_Proc_Connect),
        Net_EventHandler_New(NET_EVENT_DISCONNECT,  Net_EventClient_Proc_Disconnect),
        Net_EventHandler_New(NET_EVENT_MSG,         Net_EventClient_Proc_Msg),
        Net_EventHandler_Null()
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

    
    Net_EventClient_Update(&c);
    Net_EventClient_DoAll(&c,NULL);

	if(Stroke(ALX_KEY_ENTER).PRESSED){
        String_Add(&tb.In.Buffer,"]: ",0);
        String_AddString(&tb.In.Buffer,&nametb.In.Buffer,0);
        String_Add(&tb.In.Buffer,"[",0);

		Net_EventClient_Send(&c,NET_EVENT_MSG,tb.In.Buffer.Memory,tb.In.Buffer.size);
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
    Net_EventClient_Free(&c);
}

int main(){
    if(Create("Text:Client",1000,1000,1,1,Setup,Update,Delete))
        Start();
    return 0;
}