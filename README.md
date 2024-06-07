# **Contact Management**
## **Project Report:**

**Dependencies:** **SDL2**, **gcc**.

**How to run** :
```bash
gcc % -o %< -lm -I/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/SDL2.framework/Headers -Wl,-rpath,/Library/Frameworks -    F/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks -framework SDL2
```

### **Theme:**

Manipulation of contact management in C language. **Pair:**

- **Belkacemi Abderrahim** – (Rahim444)  
  [LinkedIn Profile](https://www.linkedin.com/in/belkacemi-abderrahim-80320a184/)

- **Aoudia Nour Islam** (islam5603)

## 1. **Description:**

Our project involves implementing a contact management system using a binary file named 'Contacts.bin'. This file adopts an LVC structure (Chained Organization, variable record format, and overlapping). This approach offers particular flexibility to efficiently manage a large number of contacts.

## **2.1. Data Structures Used:**

- **Contact:** The Contact structure is designed to store information about a contact, including an identifier (iD), a name (name), a phone number (phoneNumber), an email address (email), and the possibility to include additional variable-length information (otherInfo). Each field is sized to accommodate the maximum expected size, with margins for string termination characters. The use of a pointer for otherInfo suggests dynamic memory management for storing additional details.

- **Block:** The Block structure is intended to organize and store contacts efficiently. Each block is identified by a number (blockNumber) and keeps track of the space occupied in characters (occupiedSpace). A character array named Contacts is used to store contacts, although the specific size is not specified. A pointer to the next block (nextBlock) allows for the creation of a linked list of blocks, facilitating dynamic memory management to store a large number of contacts optimally.

- **FileInfo:** The FileInfo structure centrally manages a sequence of blocks, probably organized in a linked list to store contacts. The main elements include a pointer to the first block (firstBlock), the total number of blocks in the sequence (totalSize), the total number of contacts stored (contactSize), as well as statistics on operations performed, such as the number of contacts added (addedContacts) and the number of contacts deleted (deletedContacts).

- **IndexFile:** The IndexFile structure is designed to index data blocks in a file. Each index element is associated with a file identifier (id), a pointer to the corresponding block (Pblock), and an offset within the block (offset). The linked list (next) allows for dynamic management of index elements, thus facilitating flexible organization of multiple index entries in a file structure.

## **2.2. Implemented Modules:**

- **SDL2/SDL.h:** The \<SDL2/SDL.h\> library in C language, known as SDL (**Simple DirectMedia Layer**), offers powerful features for multimedia development. It enables the creation of graphic windows, user input management, graphic rendering, sound and music playback, as well as time and event management. SDL also facilitates working with threads for concurrent task management. Including \<SDL2/SDL.h\> in the code indicates the use of these features, but also requires linking with the SDL library during compilation for proper functionality.

## **2.3 Example Test:**

1. **After program execution:**

   ![image](https://raw.githubusercontent.com/Rahim-444/ProjectSfsd/master/assets/image1.png)

2. **Random Insertion:** Click on the 'M' key

   ![image](https://raw.githubusercontent.com/Rahim-444/ProjectSfsd/master/assets/image2.png)

3. **Insertion with contact details:** Click on 'space'.

   ![image](https://raw.githubusercontent.com/Rahim-444/ProjectSfsd/master/assets/image3.png)

4. **Deleting a contact:** Click on 'delete'

   **Before:**

   ![image](https://raw.githubusercontent.com/Rahim-444/ProjectSfsd/master/assets/image4.png)

   **After:**

   ![image](https://raw.githubusercontent.com/Rahim-444/ProjectSfsd/master/assets/image5.png)

5. **Searching for a contact:** Click on 'R'

   ![image](https://raw.githubusercontent.com/Rahim-444/ProjectSfsd/master/assets/image6.png)

6. **Contact file:**

   ![image](https://raw.githubusercontent.com/Rahim-444/ProjectSfsd/master/assets/image8.png)

7. **Sorted contact file:**

   ![image](https://raw.githubusercontent.com/Rahim-444/ProjectSfsd/master/assets/image10.png)

8. **Dense index file:**

   ![image](https://raw.githubusercontent.com/Rahim-444/ProjectSfsd/master/assets/image7.png)

9. **Sparse index file:**

   ![image](https://raw.githubusercontent.com/Rahim-444/ProjectSfsd/master/assets/image9.png)
