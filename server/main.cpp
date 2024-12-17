#include "ats.h"

int main(int argc, char *argv[]) {

    ATS ats;

    // Create some abonents
    Abonent* abonent1 = new Abonent("John Doe", "1234567890", Abonent::ConnectionStatus::Free);
    Abonent* abonent2 = new Abonent("Jane Smith", "0987654321", Abonent::ConnectionStatus::Ready);

    // Add abonents to ATS
    ats.addAbonent(abonent1);
    ats.addAbonent(abonent2);

    // Change status of an abonent
    ats.changeAbonentStatus("1234567890", Abonent::ConnectionStatus::InCall);

    // Retrieve an abonent and print their information
    Abonent* retrieved = ats.getAbonent("1234567890");
    if (retrieved) {
        qDebug() << "Retrieved abonent:" << retrieved->getName() << "with status:" << static_cast<int>(retrieved->getStatus());
    }

    // Remove an abonent
    ats.removeAbonent("0987654321");
}
