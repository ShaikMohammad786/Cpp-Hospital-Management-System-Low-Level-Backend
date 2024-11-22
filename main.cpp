#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>

using namespace std;

// Define classes for patients, doctors, and appointments
class Patient {
public:
    string name;
    int age;
    string address;
    string contact;
    string bloodGroup;
    string medicalHistory;

    Patient(string name, int age, string address, string contact,
            string bloodGroup, string medicalHistory)
        : name(name), age(age), address(address), contact(contact),
          bloodGroup(bloodGroup), medicalHistory(medicalHistory) {}

    void displayPatientInfo() const {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Address: " << address << endl;
        cout << "Contact: " << contact << endl;
        cout << "Blood Group: " << bloodGroup << endl;
        cout << "Medical History: " << medicalHistory << endl;
    }
};

class Doctor {
public:
    string name;
    string specialization;
    string availability;
    int experience;  // Experience attribute

    Doctor(string name, string specialization, string availability, int experience = 0)
        : name(name), specialization(specialization), availability(availability), experience(experience) {}

    void displayDoctorInfo() const {
        cout << "Name: " << name << endl;
        cout << "Specialization: " << specialization << endl;
        cout << "Availability: " << availability << endl;
        cout << "Experience: " << experience << " years\n";
    }
};


class Appointment {
public:
  Patient patient;
  Doctor doctor;
  string date;
  string time;

  Appointment(Patient patient, Doctor doctor, string date, string time)
      : patient(patient), doctor(doctor), date(date), time(time) {}

  void displayAppointmentInfo() const {
    cout << "Patient: " << patient.name << endl;
    cout << "Doctor: " << doctor.name << endl;
    cout << "Date: " << date << endl;
    cout << "Time: " << time << endl;
  }
};



// Function prototypes
void addPatient(vector<Patient> &patients);
void addDoctor(vector<Doctor> &doctors);
void addAppointment(vector<Patient> &patients, vector<Doctor> &doctors, vector<Appointment> &appointments);
void displayPatients(const vector<Patient> &patients);
void displayDoctors(const vector<Doctor> &doctors);
void displayAppointments(const vector<Appointment> &appointments);
void savePatientsToFile(const vector<Patient> &patients, const string &filename);
void loadPatientsFromFile(vector<Patient> &patients, const string &filename);
void saveDoctorsToFile(const vector<Doctor> &doctors, const string &filename);
void loadDoctorsFromFile(vector<Doctor> &doctors, const string &filename);
void saveAppointmentsToFile(const vector<Appointment> &appointments, const string &filename);
void loadAppointmentsFromFile(vector<Appointment> &appointments, const string &filename, const vector<Patient> &patients, const vector<Doctor> &doctors);
bool isValidTime(const string &time);
bool isValidDate(const string &date);
void searchPatientByName(const vector<Patient> &patients, const string &name);
void searchDoctorBySpecialization(const vector<Doctor> &doctors, const string &specialization);
string trim(const string &str);

int main() {
    vector<Patient> patients;
    vector<Doctor> doctors;
    vector<Appointment> appointments;

    // Load data from files
    loadPatientsFromFile(patients, "patients.txt");
    loadDoctorsFromFile(doctors, "doctors.txt");
    loadAppointmentsFromFile(appointments, "appointments.txt", patients, doctors);

    int choice;
    do {
        cout << "\nHospital Management System\n";
        cout << "1. Add Patient\n";
        cout << "2. Add Doctor\n";
        cout << "3. Add Appointment\n";
        cout << "4. Display Patients\n";
        cout << "5. Display Doctors\n";
        cout << "6. Display Appointments\n";
        cout << "7. Search Patient by Name\n";
        cout << "8. Search Doctor by Specialization\n";
        cout << "9. Save Patients to File\n";
        cout << "10. Save Doctors to File\n";
        cout << "11. Save Appointments to File\n";
        cout << "12. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();  // Clear the newline from the input buffer

        switch (choice) {
        case 1:
            addPatient(patients);
            break;
        case 2:
            addDoctor(doctors);
            break;
        case 3:
            addAppointment(patients, doctors, appointments);
            break;
        case 4:
            displayPatients(patients);
            break;
        case 5:
            displayDoctors(doctors);
            break;
        case 6:
            displayAppointments(appointments);
            break;
        case 7: {
            string searchName;
            cout << "Enter patient name to search: ";
            getline(cin, searchName);
            searchPatientByName(patients, searchName);
            break;
        }
        case 8: {
            string searchSpecialization;
            cout << "Enter doctor specialization to search: ";
            getline(cin, searchSpecialization);
            searchDoctorBySpecialization(doctors, searchSpecialization);
            break;
        }
        case 9:
            savePatientsToFile(patients, "patients.txt");
            break;
        case 10:
            saveDoctorsToFile(doctors, "doctors.txt");
            break;
        case 11:
            saveAppointmentsToFile(appointments, "appointments.txt");
            break;
        case 12:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 12);

    // Save data before exiting
    savePatientsToFile(patients, "patients.txt");
    saveDoctorsToFile(doctors, "doctors.txt");
    saveAppointmentsToFile(appointments, "appointments.txt");

    return 0;
}

void addDoctor(vector<Doctor> &doctors) {
    string name, specialization, availability;
    int experience;

    cout << "Enter doctor details:\n";
    cout << "Name: ";
    getline(cin, name);
    cout << "Specialization: ";
    getline(cin, specialization);
    cout << "Availability: ";
    getline(cin, availability);
    cout << "Experience (years): ";
    cin >> experience;
    cin.ignore();  // Clear the input buffer

    doctors.emplace_back(name, specialization, availability, experience);
    cout << "Doctor added successfully.\n";
}


void addPatient(vector<Patient> &patients) {
  string name, address, contact, bloodGroup, medicalHistory;
  int age;

  cout << "Enter patient details:\n";
  cout << "Name: ";
  getline(cin, name);
  cout << "Age: ";
  cin >> age;
  cin.ignore();
  cout << "Address: ";
  getline(cin, address);
  cout << "Contact: ";
  getline(cin, contact);
  cout << "Blood Group: ";
  getline(cin, bloodGroup);
  cout << "Medical History: ";
  getline(cin, medicalHistory);

  patients.emplace_back(name, age, address, contact, bloodGroup,
                        medicalHistory);
  cout << "Patient added successfully.\n";
}




void addAppointment(vector<Patient> &patients, vector<Doctor> &doctors,
                    vector<Appointment> &appointments) {
  int patientIndex, doctorIndex;
  string date, time;

  cout << "Enter patient index: ";
  cin >> patientIndex;
  cin.ignore();
  cout << "Enter doctor index: ";
  cin >> doctorIndex;
  cin.ignore();
  cout << "Enter date (YYYY-MM-DD): ";
  getline(cin, date);
  cout << "Enter time (HH:MM): ";
  getline(cin, time);

  if (patientIndex >= 0 && patientIndex < patients.size() && doctorIndex >= 0 &&
      doctorIndex < doctors.size() && isValidDate(date) && isValidTime(time)) {

    appointments.emplace_back(patients[patientIndex], doctors[doctorIndex],
                              date, time);
    cout << "Appointment added successfully.\n";
  } else {
    cout << "Invalid patient or doctor index, or date/time format.\n";
  }
}

void displayPatients(const vector<Patient> &patients) {
  cout << "Patient List:\n";
  for (const auto &patient : patients) {
    patient.displayPatientInfo();
    cout << endl;
  }
}

void displayDoctors(const vector<Doctor> &doctors) {
  cout << "Doctor List:\n";
  for (const auto &doctor : doctors) {
    doctor.displayDoctorInfo();
    cout << endl;
  }
}

void displayAppointments(const vector<Appointment>& appointments) {
    if (appointments.empty()) {
        cout << "No appointments available." << endl;
        return;
    }

    cout << "Appointment List:" << endl;
    for (const auto& appointment : appointments) {
        appointment.displayAppointmentInfo(); // Call the method to display appointment info
        cout << "---------------------------" << endl; // Separator for clarity
    }
}


void savePatientsToFile(const vector<Patient> &patients,
                        const string &filename) {
  ofstream outfile(filename);
  if (outfile.is_open()) {
    for (const auto &patient : patients) {
      outfile << patient.name << "," << patient.age << "," << patient.address
              << "," << patient.contact << "," << patient.bloodGroup << ","
              << patient.medicalHistory << endl;
    }
    outfile.close();
    cout << "Patients saved to file.\n";
  } else {
    cout << "Error opening file.\n";
  }
}

void loadPatientsFromFile(vector<Patient> &patients, const string &filename) {
  ifstream infile(filename);
  if (infile.is_open()) {
    string line;
    while (getline(infile, line)) {
      stringstream ss(line);
      string name, address, contact, bloodGroup, medicalHistory;
      int age;

      getline(ss, name, ',');
      ss >> age;
      ss.ignore();
      getline(ss, address, ',');
      getline(ss, contact, ',');
      getline(ss, bloodGroup, ',');
      getline(ss, medicalHistory);

      patients.emplace_back(name, age, address, contact, bloodGroup,
                            medicalHistory);
    }
    infile.close();
    cout << "Patients loaded from file.\n";
  } else {
    cout << "Error opening file.\n";
  }
}

void saveDoctorsToFile(const vector<Doctor> &doctors, const string &filename) {
  ofstream outfile(filename);
  if (outfile.is_open()) {
    for (const auto &doctor : doctors) {
      outfile << doctor.name << "," << doctor.specialization << ","
              << doctor.availability << endl;
    }
    outfile.close();
    cout << "Doctors saved to file.\n";
  } else {
    cout << "Error opening file.\n";
  }
}

// Assuming the structure looks something like this:
void loadDoctorsFromFile(vector<Doctor>& doctors, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open the doctors file." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name, specialization, availability;
        int experience;

        // Use ',' as delimiter to parse each field
        getline(ss, name, ',');
        getline(ss, specialization, ',');
        getline(ss, availability, ',');
        ss >> experience;  // Read experience as an integer

        // Create a Doctor object and add it to the list
        doctors.emplace_back(name, specialization, availability, experience);
    }

    cout << "Doctors loaded successfully." << endl;
}


void saveAppointmentsToFile(const vector<Appointment> &appointments,
                            const string &filename) {
  ofstream outfile(filename);
  if (outfile.is_open()) {
    for (const auto &appointment : appointments) {
      outfile << appointment.patient.name << "," << appointment.doctor.name
              << "," << appointment.date << "," << appointment.time << endl;
    }
    outfile.close();
    cout << "Appointments saved to file.\n";
  } else {
    cout << "Error opening file.\n";
  }
}

void loadAppointmentsFromFile(vector<Appointment> &appointments,
                              const string &filename,
                              const vector<Patient> &patients,
                              const vector<Doctor> &doctors) {
  ifstream infile(filename);
  if (infile.is_open()) {
    string line;
    while (getline(infile, line)) {
      string patientName, doctorName, date, time;

      stringstream ss(line);
      getline(ss, patientName, ',');
      getline(ss, doctorName, ',');
      getline(ss, date, ',');
      getline(ss, time);

      // Find corresponding Patient and Doctor
      Patient patient("", 0, "", "", "", "");
      Doctor doctor("", "", "");

      for (const auto &p : patients) {
        if (p.name == patientName) {
          patient = p;
          break;
        }
      }

      for (const auto &d : doctors) {
        if (d.name == doctorName) {
          doctor = d;
          break;
        }
      }

      appointments.emplace_back(patient, doctor, date, time);
    }
    infile.close();
    cout << "Appointments loaded from file.\n";
  } else {
    cout << "Error opening file.\n";
  }
}



bool isValidTime(const string &time) {
  regex timeRegex("^(?:[01]\\d|2[0-3]):[0-5]\\d$");
  return regex_match(time, timeRegex);
}

bool isValidDate(const string &date) {
  regex dateRegex("^(\\d{4})-(\\d{2})-(\\d{2})$");
  return regex_match(date, dateRegex);
}

void searchPatientByName(const vector<Patient> &patients, const string &name) {
  bool found = false;
  string lowerName = name;
  transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
            ::tolower); // Convert search term to lowercase

  for (const auto &patient : patients) {
    string patientName = patient.name;
    string lowerPatientName = patientName;
    transform(lowerPatientName.begin(), lowerPatientName.end(),
              lowerPatientName.begin(),
              ::tolower); // Convert patient name to lowercase

    // Check if the lower patient name contains the lower search term
    if (lowerPatientName.find(lowerName) != string::npos) {
      patient.displayPatientInfo();
      found = true;
      break; // If you want to stop after finding the first match
    }
  }
  if (!found) {
    cout << "Patient not found.\n";
  }
}




void searchDoctorBySpecialization(const vector<Doctor> &doctors, const string &specialization) {
    bool found = false;
    string lowerSpecialization = trim(specialization);
    transform(lowerSpecialization.begin(), lowerSpecialization.end(), lowerSpecialization.begin(), ::tolower);

    for (const auto &doctor : doctors) {
        string lowerDoctorSpecialization = trim(doctor.specialization);
        transform(lowerDoctorSpecialization.begin(), lowerDoctorSpecialization.end(), lowerDoctorSpecialization.begin(), ::tolower);

        if (lowerDoctorSpecialization == lowerSpecialization) {
            doctor.displayDoctorInfo();
            found = true;
        }
    }
    if (!found) {
        cout << "Doctor not found.\n";
    }
}


string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return ""; // No content
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}