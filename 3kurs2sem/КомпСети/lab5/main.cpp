#include <cstdlib>
#include <iostream>
#include <winsock.h>
#include <iphlpapi.h>

using namespace std;

int output_arp_table()
{
    PMIB_IPNETTABLE ip_arp_table = NULL;
    DWORD actual_size = 0;
    GetIpNetTable(ip_arp_table, &actual_size, true);
    ip_arp_table = (PMIB_IPNETTABLE)malloc(actual_size);
    if (GetIpNetTable(ip_arp_table, &actual_size, true) != NO_ERROR)
    {
        cout << "Error getting ARP table\n"
             << endl;
        if (ip_arp_table)
            free(ip_arp_table);
        return 1;
    }
    DWORD current_index;
    char type[256], address[256];
    PMIB_IPADDRTABLE ip_address_table = NULL;
    actual_size = 0;
    GetIpAddrTable(ip_address_table, &actual_size, true);
    ip_address_table = (PMIB_IPADDRTABLE)malloc(actual_size);
    GetIpAddrTable(ip_address_table, &actual_size, true);
    current_index = -1;
    for (int i = 0; i < ip_arp_table->dwNumEntries; i++)
    {
        if (ip_arp_table->table[i].dwIndex != current_index)
        {
            current_index = ip_arp_table->table[i].dwIndex;

            IN_ADDR in_address;
            for (int j = 0; j < ip_address_table->dwNumEntries; j++)
            {
                if (current_index != ip_address_table->table[j].dwIndex)
                    continue;
                in_address.S_un.S_addr = ip_address_table->table[j].dwAddr;
                strcpy(address, inet_ntoa(in_address));
            }
            printf("Interface: %s --- 0x%X\n", address, current_index);
            cout << "Internet Address   | Physical Address  | Type" << endl;
        }
        switch (ip_arp_table->table[i].dwType)
        {
        case 1:
            strcpy(type, "Other");
            break;
        case 2:
            strcpy(type, "Invalid");
            break;
        case 3:
            strcpy(type, "Dynamic");
            break;
        case 4:
            strcpy(type, "Static");
            break;
        default:
            strcpy(type, "");
        }
        IN_ADDR in_address;
        in_address.S_un.S_addr = ip_arp_table->table[i].dwAddr;
        printf("%-18s |", inet_ntoa(in_address));
        printf(" %02X:%02X:%02X:%02X:%02X:%02X | %-11s\n", ip_arp_table->table[i].bPhysAddr[0],
               ip_arp_table->table[i].bPhysAddr[1],
               ip_arp_table->table[i].bPhysAddr[2],
               ip_arp_table->table[i].bPhysAddr[3],
               ip_arp_table->table[i].bPhysAddr[4],
               ip_arp_table->table[i].bPhysAddr[5], type);
    }
    free(ip_arp_table);
    cout << endl;
    return 0;
}

int add_ip_net()
{
    char array_inet_address[255], mac_address[255], net_interface[255];
    cout << "Enter interface: ";
    cin >> net_interface;
    cout << "Enter IP address: ";
    cin >> array_inet_address;
    cout << "Enter MAC address: ";
    cin >> mac_address;
    DWORD inet_address = inet_addr(array_inet_address);
    if (inet_address == INADDR_NONE)
    {
        cout << "Invalid IP address.\n"
             << endl;
        return 1;
    }
    MIB_IPNETROW arp_row;
    sscanf(net_interface, "%x", &(arp_row.dwIndex));
    arp_row.dwPhysAddrLen = 6;
    sscanf(mac_address, "%hx:%hx:%hx:%hx:%hx:%hx",
           &arp_row.bPhysAddr[0],
           &arp_row.bPhysAddr[1],
           &arp_row.bPhysAddr[2],
           &arp_row.bPhysAddr[3],
           &arp_row.bPhysAddr[4],
           &arp_row.bPhysAddr[5]);
    arp_row.dwAddr = inet_address;
    arp_row.dwType = MIB_IPNET_TYPE_STATIC;
    switch (CreateIpNetEntry(&arp_row))
    {
    case ERROR_ACCESS_DENIED:
        cout << "Entry not added. Access denied" << endl;
        break;
    case NO_ERROR:
        cout << "Entry added" << endl;
        break;
    default:
        cout << "Entry not added" << endl;
    }
    return 0;
}

int del_ip_net()
{
    char array_inet_address[255], net_interface[255];
    cout << "Enter interface: ";
    cin >> net_interface;
    cout << "Enter IP address: ";
    cin >> array_inet_address;
    DWORD inet_address = inet_addr(array_inet_address);
    if (inet_address == INADDR_NONE)
    {
        cout << "Invalid IP address" << endl;
        return 1;
    }
    MIB_IPNETROW arp_row;
    sscanf(net_interface, "%x", &(arp_row.dwIndex));
    arp_row.dwAddr = inet_address;

    switch (DeleteIpNetEntry(&arp_row))
    {
    case ERROR_ACCESS_DENIED:
        cout << "Entry not deleted. Access denied" << endl;
        break;
    case NO_ERROR:
        cout << "Entry deleted" << endl;
        break;
    default:
        cout << "Entry not deleted" << endl;
    }
    return 0;
}

void get_mac_by_ip()
{
    DWORD actual_size = 0;
    PMIB_IPNETTABLE ip_address_table = NULL;
    GetIpNetTable(ip_address_table, &actual_size, true);
    ip_address_table = (PMIB_IPNETTABLE)malloc(actual_size);
    GetIpNetTable(ip_address_table, &actual_size, true);

    char array_inet_address[255];
    cout << "Enter IP address: ";
    cin >> array_inet_address;
    DWORD inet_address = inet_addr(array_inet_address);
    if (inet_address == INADDR_NONE)
    {
        cout << "Invalid IP address" << endl;
        return;
    }

    bool search_flag = true;
    for (int i = 0; i < ip_address_table->dwNumEntries; i++)
    {
        if (inet_address == ip_address_table->table[i].dwAddr)
        {
            printf("MAC address: %02X:%02X:%02X:%02X:%02X:%02X on interface with index %x\n",
                   ip_address_table->table[i].bPhysAddr[0],
                   ip_address_table->table[i].bPhysAddr[1],
                   ip_address_table->table[i].bPhysAddr[2],
                   ip_address_table->table[i].bPhysAddr[3],
                   ip_address_table->table[i].bPhysAddr[4],
                   ip_address_table->table[i].bPhysAddr[5], ip_address_table->table[i].dwIndex);
            search_flag = false;
        }
    }
    if (search_flag)
        cout << "No matches found" << endl;
}

int main(int argc, char *argv[])
{
    int selected_value;
    bool is_run = true;
    while (is_run)
    {
        cout << "Choose an action:" << endl
             << "  1. Show ARP table" << endl
             << "  2. Add an entry" << endl
             << "  3. Delete an entry" << endl
             << "  4. Get MAC by IP" << endl
             << "  5. Exit" << endl;
        cin >> selected_value;
        switch (selected_value)
        {
        case 1:
            output_arp_table();
            break;
        case 2:
            add_ip_net();
            break;
        case 3:
            del_ip_net();
            break;
        case 4:
            get_mac_by_ip();
            break;
        case 5:
        default:
            is_run = false;
            break;
        }
    }

    return 0;
}