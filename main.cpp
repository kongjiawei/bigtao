#include<afxtempl.h>
#include <atlstr.h>
#include "TeleApi.h"
#include<iostream>

//ע�����.lib��
int main() {

    ULONG error = 1;
    ULONG ulChassisId = 0;
    ULONG ulSlotId = 2;
    ULONG ulPortId = 1;
    UINT64 frameRate = 0;
    UINT64 last = 0;
    int i = 1;

    /*Regular Operation srart*/
    InitAllModules();  //��ʼ������ģ�飬��Ҫ�ڳ������ڴ����øú���
    error = ResChassisAdd(RES_CHASSIS_TYPE_220, 0xC0A86CB4, &ulChassisId); //���һ���ͺ�δRES_CHASSIS_TYPE_220, ����IP��ַΪ192.168.108.180�Ļ���
    if (error != 0) {
        std::cout << "Error Code: " << error << std::endl;
        exit(error);
    }
    error = ResCardAdd(ulChassisId, ulSlotId, RES_CARD_TYPE_V8002F); //�ڻ���ĵ�һ����λ���һ��RES_CARD_TYPE_V8002F�Ľӿڿ�
    if (error != 0) {
        std::cout << "Error Code: " << error << std::endl;
        exit(error);
    }
    std::cout << "Card Added \n" << std::endl;


    error = ResChassisConnect(ulChassisId); //��ָ�����佨��TCP����
    if (error != 0) {
        std::cout << "Error Code: " << error << std::endl;
        exit(error);
    }
    std::cout << "Chassis connected \n" << std::endl;


    error = ResPortReserve(ulChassisId, ulSlotId, ulPortId); //ԤԼָ�������һ��λ�ϵĵ�һ���˿�
    if (error != 0) {
        std::cout << "Error Code: " << error << std::endl;
        exit(error);
    }
    std::cout << "Port Reserved \n" << std::endl;

    error = TraEnablePort(ulChassisId, ulSlotId, ulPortId, TRUE); //ʹ��ָ�������һ��λ�ϵ�һ���˿ڵ������͹���
    if (error != 0) {
        std::cout << "Error Code: " << error << std::endl;
        exit(error);
    }
    std::cout << "Stream send enabled \n" << std::endl;
    /*Regular Operation end*/

    /*Setup start*/
    error = TraSetPortTransmitMode(ulChassisId, ulSlotId, ulPortId, TRA_TX_MODE_CONTINUOUS, 0, 0, 0); //��ָֻ�������һ��λ�Ͻӿڿ��ĵ�һ�˿ڵķ���ģʽΪ�������ͣ�������������������������Ч��
    /*
    mode to be select :
    TRA_TX_MODE_CONTINUOUS��ʾ�������ͣ�
    TRA_TX_MODE_SINGLE_BURST��ʾ����ͻ����
    TRA_TX_MODE_MULTI_BURST��ʾ���ͻ��
    TRA_TX_MODE_TIME_BURST��ʾ��ʱ�䷢��
    */ 
    if (error != 0) {
        std::cout << "Error Code: " << error << std::endl;
        exit(error);
    }
    std::cout << "Transmit Mode set \n" << std::endl;

    error = TraSetPortScheduleMode(ulChassisId, ulSlotId, ulPortId, TRA_SCHEDULE_MODE_IFG); //����ָ�������һ��λ�Ͻӿڿ��ĵ�һ�˿ڵ������ȷ�ʽ
    /*
    TRA_SCHEDULE_MODE_IFG�����ڶ˿ڵ��٣�
    TRA_SCHEDULE_MODE_FPS�����������٣�
    */
    if (error != 0) {
        std::cout << "Error Code: " << error << std::endl;
        exit(error);
    }
    std::cout << "Schedule Mode set \n" << std::endl;


    error = TraAddStream(ulChassisId, ulSlotId, ulPortId, "s1", TRA_PRO_TYPE_IPV4, 1, TRA_LEN_TYPE_FIXED, 1000, 1000, FALSE, TRA_PAY_TYPE_CYCLE, 0x5a,
        0, 0x000001020304, 0x000001020305, 0x01000001, 0x01000002, NULL, NULL); //ָ�������һ��λ�ϵ�һ�˿������һ����s1, ����Ϊipv4���ģ� �̶����ȣ�����Ϊ
	// 1000�ֽڣ�ԴMAC��ַΪ00-00-00-01-02-03-04�� Ŀ��MAC��ַΪ00-00-01-02-03-05; Դipv4��ַΪ1.0.0.1��Ŀ��IPV4��ַΪ1.0.0.2��
    if (error != 0) {
        std::cout << "Error Code: " << error << std::endl;
        exit(error);
    }

    error = TraEnableStream("s1", TRUE); //ʹ��s1��
    if (error != 0) {
        std::cout << "Error Code: " << error << std::endl;
        exit(error);
    }

    error = TraSetEthernetPortRate(ulChassisId, ulSlotId, ulPortId, TRA_RATE_UNIT_TYPE_FPS, 200000); //����ָ�������һ��λ�Ͻӿڿ��ĵ�һ�˿ڵ�����������Ϊ200000FPS
    if (error != 0) {
        std::cout << "Error Code: " << error << std::endl;
        exit(error);
    }
    error = StaSelectStream(ulChassisId, ulSlotId, ulPortId, "s1"); //����s1����ָ�������һ��λ�µ�һ�˿��½���ͳ��
    if (error != 0) {
        std::cout << "Error Code: " << error << std::endl;
        exit(error);
    }

    error = StaStartPort(ulChassisId, ulSlotId, ulPortId); //��ʼָ�������һ��λ�Ͻӿڿ��ĵ�һ�˿ڵ�ͳ��
    if (error != 0) {
        std::cout << "Error Code: " << error << std::endl;
        exit(error);
    }
    TraStart();
    Sleep(1000);
    for (int i = 2; i <= 10; i++) {
        error = StaGetPortData(ulChassisId, ulSlotId, ulPortId, STA_MEA_TX_FPS, &frameRate); //��ȡָ�������һ��λ�Ͻӿڿ��ĵ�һ�˿ڵķ��͵�����֡��ͳ�ƣ������frameRate��
        if (error != 0) {
            std::cout << "Error Code: " << error << std::endl;
            exit(error);
        }

        std::cout << "Frame rate is " << frameRate<< " frm/sec" << std::endl;
        last = frameRate;

        // Dynamically adjust frame rate 
        error = TraSetEthernetPortRate(ulChassisId, ulSlotId, ulPortId, TRA_RATE_UNIT_TYPE_FPS, i*200000); //����ָ�������һ��λ�ӿڿ��ĵ�һ�˿ڵ�����������Ϊi*200000
        if (error != 0) {
            std::cout << "Error Code: " << error << std::endl;
            exit(error);
        }
        Sleep(1000);
    }
    TraStop(); //ֹͣ������
    StaStopPort(ulChassisId, ulSlotId, ulPortId); //ָֹͣ���˿ڵ�ͳ��

    // remember to release the resources
    TraRemoveStream("s1"); //ɾ��ָ������
    ResPortRelease(ulChassisId, ulSlotId, ulPortId); //�ͷ�ָ���Ľӿڿ��Ķ˿�
	DeleteAllModules();//�ͷ�������Դ
    return 0;
}
