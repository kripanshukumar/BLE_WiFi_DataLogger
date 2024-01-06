class BLE_DEVICE
{
private:
    void construct_json()
    {
        if(json_string != NULL) {
            delete[] json_string;
        }
        int len = strlen(name) + strlen(Address) + strlen(manf_data) + strlen(serviceUUID) + strlen(json_filler) - 5;
        json_string = new char[len + 1];
        sprintf(json_string, json_filler, name, Address, manf_data, serviceUUID, rssi);
    }

public:
    static int count;
    char* name;
    char* Address;
    char* manf_data;
    char* serviceUUID;
    int rssi;
    int id;
    static const char* json_filler;
    char* json_string;

    BLE_DEVICE()
    {
        // Serial.println("\nConstructor is invoked");
        name = NULL;
        Address = NULL;
        manf_data = NULL;
        serviceUUID = NULL;
        rssi = 0;
        json_string = NULL;
        id = count;
        count++;
    }

    BLE_DEVICE(const char* name, const char* Address, const char* manf_data, const char* serviceUUID, int rssi)
        : BLE_DEVICE()
    {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
        this->Address = new char[strlen(Address) + 1];
        strcpy(this->Address, Address);
        this->manf_data = new char[strlen(manf_data) + 1];
        strcpy(this->manf_data, manf_data);
        this->serviceUUID = new char[strlen(serviceUUID) + 1];
        strcpy(this->serviceUUID, serviceUUID);
        this->rssi = rssi;
        construct_json();
    }

    ~BLE_DEVICE()
    {
        // Serial.println("\nThe Destructor is invoked");
        // Serial.print("ID: ");Serial.println(id);
        // Serial.print("name: ");Serial.println(name);
        // Serial.print("Address: ");Serial.println(Address);
        // Serial.print("manf_data: ");Serial.println(manf_data);
        // Serial.print("serviceUUID: ");Serial.println(serviceUUID);
        delete name;
        delete Address;
        delete manf_data;
        delete serviceUUID;
        delete json_string;
        count = count - 1;
    }

    void print()
    {
        Serial.println("----------------------------------------------------------");
        Serial.print("Name            :   ");
        Serial.println(name);
        Serial.print("Address         :   ");
        Serial.println(Address);
        Serial.print("MANF_DATA       :   ");
        Serial.println(manf_data);
        Serial.print("SERVICE_UUID    :   ");
        Serial.println(serviceUUID);
        Serial.print("RSSI            :   ");
        Serial.println(rssi);
        Serial.println("----------------------------------------------------------");
    }

    bool operator==(BLE_DEVICE const& rhs)
    {
        if(strcmp(this->Address, rhs.Address) == 0) {
            return true;
        } else {
            return false;
        }
    }

    char* json()
    {
        construct_json();
        return json_string;
    }

    void operator=(BLE_DEVICE const& rhs)
    {
        // Serial.println("\nIn the assignment operator....");
        if(name != NULL) {
            delete name;
        }
        if(Address != NULL) {
            delete Address;
        }
        if(manf_data != NULL) {
            delete manf_data;
        }
        if(serviceUUID != NULL) {
            delete serviceUUID;
        }
        if(json_string != NULL) {
            delete json_string;
        }
        this->name = new char[strlen(rhs.name) + 1];
        strcpy(this->name, rhs.name);
        this->Address = new char[strlen(rhs.Address) + 1];
        strcpy(this->Address, rhs.Address);
        this->manf_data = new char[strlen(rhs.manf_data) + 1];
        strcpy(this->manf_data, rhs.manf_data);
        this->serviceUUID = new char[strlen(rhs.serviceUUID) + 1];
        strcpy(this->serviceUUID, rhs.serviceUUID);
        this->rssi = rhs.rssi;
        construct_json();
    }

    BLE_DEVICE(const BLE_DEVICE& rhs)
    {
        // Serial.println("\nIn the copy constructor....");
        this->name = new char[strlen(rhs.name) + 1];
        strcpy(this->name, rhs.name);
        this->Address = new char[strlen(rhs.Address) + 1];
        strcpy(this->Address, rhs.Address);
        this->manf_data = new char[strlen(rhs.manf_data) + 1];
        strcpy(this->manf_data, rhs.manf_data);
        this->serviceUUID = new char[strlen(rhs.serviceUUID) + 1];
        strcpy(this->serviceUUID, rhs.serviceUUID);
        this->rssi = rhs.rssi;
        json_string = NULL;
        construct_json();
    }
};

const char* BLE_DEVICE::json_filler =
    "{\"NAME\":\"%s\",\"ADDRESS\":\"%s\",\"MFG DATA\":\"%s\",\"SERVICE UUID\":\"%s\",\"RSSI\":%d}";

int BLE_DEVICE::count = 0;


void indent(int indentation){
    Serial.println();
    for(int i = 0; i< indentation; i++){
        Serial.print(' ');
    }
}

void print_json(char* str)
{   
    int indentation = 0;
    int index = 0;
    while(index <strlen(str)){
        if(str[index] == '{' || str[index] == '['){
            Serial.print(char(str[index]));
            indentation += 3;
            indent(indentation);
        }
        else if(str[index] == '}' || str[index] == ']'){
            indentation -= 3;
            indent(indentation);
            Serial.print(char(str[index]));
        }
        else if(str[index] == ','){
            Serial.print(char(str[index]));
            indent(indentation);
        }
        /*else if(str[index] == ':'){
            Serial.print(char(str[index]));
            Serial.print(' ');
        }
        */
        else{
            Serial.print(char(str[index]));
        }
        index++;
    }
    Serial.println();
}
