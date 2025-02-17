#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include "IPX.H"

unsigned IntSwap(unsigned i) {
    return ((i >> 8) | (i & 0xFF) << 8);
}

int IPXOpenSocket(int socketType, unsigned* socket) {
    union REGS inregs, outregs;
    struct SREGS segregs;

    inregs.x.bx = IPX_CMD_OPEN_SOCKET;
    inregs.x.dx = IntSwap(*socket);
    inregs.x.ax = socketType;

    int86x(0x7A, &inregs, &outregs, &segregs);

    *socket = IntSwap(outregs.x.dx);
    return outregs.x.ax;
}

void IPXCloseSocket(unsigned* socket) {
    union REGS inregs, outregs;
    struct SREGS segregs;

    inregs.x.bx = IPX_CMD_CLOSE_SOCKET;
    inregs.x.dx = IntSwap(*socket);

    int86x(0x7A, &inregs, &outregs, &segregs);
}

void IPXListenForPacket(struct ECB* rxECB) {
    union REGS inregs, outregs;
    struct SREGS segregs;

    segregs.es = FP_SEG((void far*)rxECB);
    inregs.x.si = FP_OFF((void far*)rxECB);
    inregs.x.bx = IPX_CMD_LISTEN_FOR_PACKET;

    int86x(0x7A, &inregs, &outregs, &segregs);
}

void IPXSendPacket(struct ECB* txECB) {
    union REGS inregs, outregs;
    struct SREGS segregs;

    segregs.es = FP_SEG((void far*)txECB);
    inregs.x.si = FP_OFF((void far*)txECB);
    inregs.x.bx = IPX_CMD_SEND_PACKET;

    int86x(0x7A, &inregs, &outregs, &segregs);
}

void IPXRelinquishControl(void) {
    union REGS inregs, outregs;
    struct SREGS segregs;

    inregs.x.bx = IPX_CMD_RELINQUISH_CONTROL;

    int86x(0x7A, &inregs, &outregs, &segregs);
}

void PrintECB(const struct ECB* ecb) {
    unsigned int i;
    printf("ECB Structure:\n");
    printf("  Link: %Fp\n", ecb->Link); // %Fp для far-указателя
    printf("  ESRAddress: %Fp\n", ecb->ESRAddress); // %Fp для far-указателя
    printf("  InUse: 0x%02X\n", ecb->InUse);
    printf("  CCode: 0x%02X\n", ecb->CCode);
    printf("  Socket: 0x%04X\n", ecb->Socket);
    printf("  ConnectionId: 0x%04X\n", ecb->ConnectionId);
    printf("  RestOfWorkspace: 0x%04X\n", ecb->RestOfWorkspace);

    printf("  DriverWorkspace: ");
    i = 0;
    while(i < 12) {
	printf("%02X ", ecb->DriverWorkspace[i]);
	i++;
    }
    printf("\n");

    printf("  ImmAddress: ");
    i = 0;
    while(i < 6) {
	printf("%02X ", ecb->ImmAddress[i]);
	i++;
    }
    printf("\n");

    printf("  FragmentCnt: %u\n", ecb->FragmentCnt);
    i = 0;
    while (i < 2) {
	printf("  Packet[%d]:\n", i);
	printf("    Address: %Fp\n", ecb->Packet[i].Address); // %Fp для far-указателя
	printf("    Size: %u\n", ecb->Packet[i].Size);
	i++;
    }
}