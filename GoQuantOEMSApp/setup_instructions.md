# Project Setup Documentation

## Introduction
This document provides step-by-step instructions on how to install and configure the necessary dependencies for running the project on a **Windows** system. The dependencies are managed using **vcpkg**, which simplifies the installation of required C++ libraries such as **Drogon** and **websocketpp**.

### Dependencies:
- **vcpkg** (for managing C++ dependencies)
- **Drogon** (for HTTP client and server functionality)
- **websocketpp** (for WebSocket server functionality)
- **Flatbuffers** (for efficient data serialization)
- **Intel TBB** (optional, for multithreading performance)

## 1. Install **vcpkg**

**vcpkg** is a C++ package manager that simplifies the installation of third-party libraries. Follow the steps below to install **vcpkg** on your system.

### Step-by-Step Instructions for Installing vcpkg:

1. **Open PowerShell** as **Administrator**.
   - Search for **PowerShell** in the Windows Start menu, right-click on it, and select **Run as Administrator**.

2. **Clone the vcpkg Repository**:
   - Install **vcpkg** in a central location on your system (e.g., `C:\dev\vcpkg`).
     ```powershell
     git clone https://github.com/microsoft/vcpkg.git C:\dev\vcpkg
     cd C:\dev\vcpkg
     ```

3. **Bootstrap vcpkg**:
   - Run the following command to build and set up **vcpkg**:
     ```powershell
     .\bootstrap-vcpkg.bat
     ```

4. **Integrate vcpkg with Visual Studio**:
   - This step makes sure that any libraries installed by **vcpkg** will be automatically available to Visual Studio:
     ```powershell
     .\vcpkg integrate install
     ```

---

## 2. Install Required Libraries

The project uses several external libraries managed via **vcpkg**. Follow these steps to install them:

### Step-by-Step Instructions for Installing Libraries:

1. **Install Drogon** (for HTTP client and server):
   ```powershell
   .\vcpkg install drogon
   ```

2. **Install websocketpp** (for WebSocket server functionality):
   ```powershell
   .\vcpkg install websocketpp
   ```

3. **Install Flatbuffers** (for efficient data serialization):
   ```powershell
   .\vcpkg install flatbuffers
   ```

4. **(Optional) Install Intel TBB** (for multithreading performance):
   ```powershell
   .\vcpkg install tbb
   ```

### Notes:
- After installation, **vcpkg** will ensure that these libraries are automatically linked in your project when using **Visual Studio**.
- If you need to install any additional libraries, you can follow the same process by using the command `.\vcpkg install <library_name>`.

---

## 3. Verify Installation

Once the dependencies are installed, follow these steps to verify that everything is set up correctly.

### Verifying Drogon and Websocketpp Installation:
1. **Open Visual Studio** and load the project.
2. **Include Drogon and websocketpp in your code**:
   ```cpp
   #include <drogon/drogon.h>
   #include <websocketpp/config/asio_no_tls.hpp>
   ```

3. **Build the Project**:
   - In Visual Studio, click on **Build > Build Solution**.
   - Ensure that no errors related to missing libraries appear during the build process.
   - If successful, the project will compile, and the necessary dependencies will be linked automatically.

---

## 4. Running the Program

After verifying that all dependencies are correctly installed and the project builds successfully, you can run the program. Here’s how:

1. **Run from Visual Studio**:
   - Open the solution in **Visual Studio**.
   - Click on **Debug > Start Without Debugging** or simply press `Ctrl + F5` to run the program.

2. **Command-Line Execution**:
   - If you prefer to run the compiled executable from the command line, navigate to the **output directory** (e.g., `Debug` or `Release` folder) and execute the program:
     ```bash
     .\GoQuantOEMSApp.exe
     ```

---

## 5. Additional Notes

- **Cross-Platform Usage**: If the project needs to run on Linux or macOS, **vcpkg** also supports those platforms. The steps are similar, with the exception of using different commands to install dependencies (`./vcpkg install drogon` on Linux/macOS).
  
- **Updating Libraries**: To update any library installed via **vcpkg**, you can pull the latest changes from the **vcpkg** repository and run the following command:
  ```powershell
  .\vcpkg upgrade
  ```

- **Custom Configurations**: If any additional configurations are needed for a specific environment, ensure that **vcpkg** integrates with the development environment by running the `.\vcpkg integrate install` command again.

---

## Contact Information

If you encounter any issues during the setup or installation process, feel free to reach out for further assistance.

---