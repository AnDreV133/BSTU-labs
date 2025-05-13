#include <iostream>
#include <WinSock.h>
#include <IPHlpApi.h>
#include <string>

#define STR_BUF_SIZE 1024

using namespace std;

enum AddressControllerMode
{
    ASSIGN_ADDRESS,
    RELEASE_ADDRESS
};

void throw_err_with_code()
{
    std::string err_msg = "Error with code: ";
    err_msg += to_string(WSAGetLastError());
    throw std::runtime_error(err_msg);
}

void view_ip()
{
    PIP_ADAPTER_INFO adapter_info, adapter;
    ULONG adapter_info_value;
    int error;
    adapter_info_value = 0;
    error = GetAdaptersInfo(NULL, &adapter_info_value);
    if ((error != 0) && (error != ERROR_BUFFER_OVERFLOW))
    {
        cout << "Buffer overflow" << endl;
        return;
    }
    if ((adapter_info = (PIP_ADAPTER_INFO)GlobalAlloc(GPTR, adapter_info_value)) == NULL)
    {
        cout << "Memory allocation error" << endl;
        return;
    }
    if (GetAdaptersInfo(adapter_info, &adapter_info_value) != 0)
    {
        cout << "Unknown error" << endl;
        return;
    }
    adapter = adapter_info;
    while (adapter)
    {
        cout << "Adapter: " << adapter->AdapterName
             << "\nIndex: " << adapter->Index
             << "\nDHCP enabled: " << (adapter->DhcpEnabled ? "yes" : "no")
             << "\nDHCP address: " << adapter->DhcpServer.IpAddress.String << endl;
        PIP_ADDR_STRING address = &(adapter->IpAddressList);
        while (address)
        {
            cout << "----------" << endl;
            cout << "Context: " << address->Context << endl;
            cout << "IP address: " << address->IpAddress.String << endl;
            cout << "Mask: " << address->IpMask.String << endl;
            address = address->Next;
        }
        cout << endl
             << endl;
        adapter = adapter->Next;
    }
}

void control_ip_address(AddressControllerMode mode)
{
    unsigned int index;
    cout << "Enter adapter index "
         << (mode == ASSIGN_ADDRESS ? "to assign new address" : "to release address")
         << endl;
    cin >> index;

    DWORD interface_info_size = 0;
    PIP_INTERFACE_INFO interface_info;
    if (GetInterfaceInfo(NULL, &interface_info_size) != ERROR_INSUFFICIENT_BUFFER)
    {
        cerr << "Buffer error" << endl;
        return;
    }
    if ((interface_info = (PIP_INTERFACE_INFO)GlobalAlloc(GPTR, interface_info_size)) == NULL)
    {
        cerr << "Not enough memory" << endl;
        return;
    }
    if (GetInterfaceInfo(interface_info, &interface_info_size) != 0)
    {
        cerr << "Failed to get table" << endl;
        return;
    }
    for (int i = 0; i < interface_info->NumAdapters; ++i)
    {
        if (index == interface_info->Adapter[i].Index)
        {
            switch (mode)
            {
            case ASSIGN_ADDRESS:
                if (IpRenewAddress(&interface_info->Adapter[i]) != 0)
                {
                    cerr << "Failed to assign address" << endl;
                    return;
                }
                break;
            case RELEASE_ADDRESS:
                if (IpReleaseAddress(&interface_info->Adapter[i]) != 0)
                {
                    cerr << "Failed to release address" << endl;
                    return;
                }
                break;
            }
        }
    }
}

int main(int args, char *argv[])
{
    int selected_value = 1;
    while (selected_value != 4)
    {
        cout << "Select action:\n"
             << "  1. Show table\n"
             << "  2. Assign address\n"
             << "  3. Release address\n"
             << "  4. Exit" << endl;
        cin >> selected_value;

        switch (selected_value)
        {
        case 1:
            view_ip();
            break;
        case 2:
            control_ip_address(ASSIGN_ADDRESS);
            break;
        case 3:
            control_ip_address(RELEASE_ADDRESS);
        default:
            break;
        }
    }

    return 0;
}