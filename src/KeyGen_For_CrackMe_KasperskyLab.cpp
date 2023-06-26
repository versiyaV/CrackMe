#include <stdio.h>
#include <stdint.h>
#include <string>
#include <iostream>
using namespace std;

uint64_t __PAIR64__(uint32_t val1, uint32_t val2)
{
    return uint64_t(val1) << 32 | val2;
}

void get_new_magic_number(const char* email, uint32_t email_len, uint32_t& magic_number_1, uint32_t& magic_number_2)
{
    magic_number_1 = 0x4552534B;
    magic_number_2 = 0x4B415350;
    for (uint32_t i = 0; i < email_len; i++)
    {
        uint32_t hash_1 = (uint32_t)email[i] + magic_number_1;
        uint32_t hash_2 = ((uint32_t)email[i] + __PAIR64__(magic_number_2, magic_number_1)) >> 32;
        magic_number_2 = __PAIR64__(hash_2, hash_1) >> 28;
        magic_number_1 = 0x10 * hash_1;
    }
}


bool check_key(const char* email, uint32_t email_len, uint8_t* serial)
{
    uint32_t magic_number_1;
    uint32_t magic_number_2;
    for (int i = 0; i < 10; i++)
        if (!serial[i])
            return false;

    get_new_magic_number(email, email_len, magic_number_1, magic_number_2);

    serial[0] ^= serial[9];

    if (*(uint32_t*)serial == magic_number_1 && *((uint32_t*)serial + 1) == magic_number_2)
        return true;

    return false;
}


void generate_key(const char* email, uint32_t email_len, uint8_t* serial)
{
    uint32_t magic_number_1;
    uint32_t magic_number_2;
    get_new_magic_number(email, email_len, magic_number_1, magic_number_2);

    *(uint32_t*)(&serial[0]) = magic_number_1;
    *(uint32_t*)(&serial[4]) = magic_number_2;
    
    if (serial[0] == 1)
    {
        serial[9] = 5;      
        serial[0] ^= serial[9];
    }
    else
        serial[0] ^= serial[9];
}

void hello()
{
    cout << "#    #               #####                    #####                              #     #" << endl;
    cout << "#   #  ###### #   # #     # ###### #    #    #     # #####    ##    ####  #    # ##   ## ######" << endl;
    cout << "#  #   #       # #  #       #      ##   #    #       #    #  #  #  #    # #   #  # # # # #" << endl;
    cout << "###    #####    #   #  #### #####  # #  #    #       #    # #    # #      ####   #  #  # #####" << endl;
    cout << "#  #   #        #   #     # #      #  # #    #       #####  ###### #      #  #   #     # #" << endl;
    cout << "#   #  #        #   #     # #      #   ##    #     # #   #  #    # #    # #   #  #     # #" << endl;
    cout << "#    # ######   #    #####  ###### #    #     #####  #    # #    #  ####  #    # #     # ######" << endl;
    cout << " \n\n\n[*] Usage: enter G for generate key, enter C for check key, which had been generated, enter E for exit" << endl;
}

void bye()
{
    cout << "#    #                                                        #" << endl;
    cout << "#   #    ##    ####  #####  ###### #####   ####  #    # #   # #         ##   #####" << endl;
    cout << "#  #    #  #  #      #    # #      #    # #      #   #   # #  #        #  #  #    #" << endl;
    cout << "###    #    #  ####  #    # #####  #    #  ####  ####     #   #       #    # #####" << endl;
    cout << "#  #   ######      # #####  #      #####       # #  #     #   #       ###### #    #" << endl;
    cout << "#   #  #    # #    # #      #      #   #  #    # #   #    #   #       #    # #    #" << endl;
    cout << "#    # #    #  ####  #      ###### #    #  ####  #    #   #   ####### #    # #####" << endl;
}

void message()
{
    cout << " [*] {G} - generate key" << endl;
    cout << " [*] {C} - check key" << endl;
    cout << " [*] {E or Q} - exit" << endl;
}
int main()
{
    hello();
    message();
    std::string command;
    bool flag = false;
    uint8_t serial[10] = { 1,1,1,1,1,1,1,1,1,1 };

    while (cin >> command)
    {
        if (!command.size())
            continue;
        switch (command[0])
        {
        case 'G': case 'g':
        {
            std::string email;

            cout << "Email: ";
            cin >> email;

            generate_key(email.c_str(), email.size(), serial);

            cout << "key:";
            for (uint8_t s_idx = 0; s_idx < sizeof(serial); s_idx++)
            {
                printf("%02x", serial[s_idx]);
            }

            cout << endl;
            break;
        }
        case 'C': case 'c':
        {
            std::string email;
            std::string key;

            cout << "Email: ";  cin >> email;
            cout << "Key: ";  cin >> key;

            if (key.size() != 20)
            {
                cout << "Invalid key!" << endl;
                continue;
            }

            char tmp[3];
            tmp[2] = '\0';
            uint8_t len_buffer = 0;

            for (int i = 0; i < key.size(); i += 2)
            {
                tmp[0] = key[i];
                tmp[1] = key[i + 1];
                serial[len_buffer] = strtoul(tmp, NULL, 16);
                len_buffer++;
            }

            if (check_key(email.c_str(), email.size(), serial))
                cout << "Valid key!" << endl;
            else
                cout << "Invalid key!" << endl;
            break;
        }
        case 'E':case 'e':case 'Q':case 'q':
            flag = true;
            break;
        }
        if (flag)
            break;
        message();
    }
    bye();
    return 0;
}