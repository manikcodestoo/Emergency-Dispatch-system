#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define USERS_FILE "users.txt"
#define CASES_FILE "cases.txt"
#define UNITS_FILE "units.txt"
#define MAX_CASES 1200

struct CaseRecord
{
    int id;
    char caller[50];
    char type[30];
    int priority;
    char status[20];
    char unit[20];
};

struct UserRecord
{
    char username[80];
    char password[20];
};

struct Units
{
    int police;
    int fire;
    int ambulance;
};

void create_units_file()
{
    FILE *f = fopen(UNITS_FILE, "r");
    if (f)
    {
        fclose(f);
        return;
    }
    f = fopen(UNITS_FILE, "w");
    if (!f)
        return;
    fprintf(f, "%d %d %d\n", 5, 3, 4);
    fclose(f);
}

struct Units readUnits()
{
    struct Units u;
    FILE *f = fopen(UNITS_FILE, "r");
    if (!f)
    {
        u.police = 0;
        u.fire = 0;
        u.ambulance = 0;
        return u;
    }
    if (fscanf(f, "%d %d %d", &u.police, &u.fire, &u.ambulance) != 3)
    {
        u.police = 0;
        u.fire = 0;
        u.ambulance = 0;
    }
    fclose(f);
    return u;
}

void write_units(struct Units u)
{
    FILE *f = fopen(UNITS_FILE, "w");
    if (!f) return;
    fprintf(f, "%d %d %d\n", u.police, u.fire, u.ambulance);
    fclose(f);
}

void registerDispatcher()
{
    struct UserRecord u;
    printf("\n*Register dispatcher*\n");
    printf("Enter username without spaces: ");
    if (scanf("%50s", u.username) != 1)
    {
        while(getchar()!='\n');
        return;
    }
    while (getchar() != '\n');

    if (strcmp(u.username, "admin") == 0)
    {
        printf("ERROR: you can't use 'admin'.\n");
        return;
    }

    printf("Enter password without spaces: ");
    if (scanf("%19s", u.password) != 1)
    {
        while(getchar()!='\n');
        return;
    }
    while (getchar() != '\n');

    FILE *f = fopen(USERS_FILE, "a");
    if (!f)
    {
        printf("Error opening users file.\n");
        return;
    }
    fprintf(f, "%s %s\n", u.username, u.password);
    fclose(f);
    printf("dispatcher '%s' registered.\n", u.username);
}

int login(char *username_out, char *role_out)
{
    char username[80], password[80];
    printf("\n*Login*\n");
    printf("Username: ");
    if (scanf("%79s", username) != 1) return 0;
    while (getchar() != '\n');
    printf("Password: ");
    if (scanf("%79s", password) != 1) return 0;
    while (getchar() != '\n');

    if (strcmp(username, "admin") == 0)
    {
        if (strcmp(password, "imtheBOSS") == 0)
        {
            strcpy(role_out, "admin");
            strcpy(username_out, "admin");
            return 1;
        }
        else return 0;
    }

    FILE *f = fopen(USERS_FILE, "r");
    if (!f) return 0;
    char u[80], p[20];
    int ok = 0;
    while (fscanf(f, "%79s %19s", u, p) == 2)
    {
        if (strcmp(u, username) == 0 && strcmp(p, password) == 0)
        {
            ok = 1;
            break;
        }
    }
    fclose(f);
    if (ok)
    {
        strcpy(role_out, "dispatcher");
        strcpy(username_out, username);
        return 1;
    }
    return 0;
}

void append_case_to_file(struct CaseRecord c)
{
    FILE *f = fopen(CASES_FILE, "a");
    if (!f)
    {
        printf("Error: cannot open cases file for appending!\n");
        return;
    }
    fprintf(f, "%d %s %s %d %s %s\n",
            c.id, c.caller, c.type, c.priority, c.status, c.unit);
    fclose(f);
}

int load_all_cases(struct CaseRecord arr[], int max_size)
{
    FILE *f = fopen(CASES_FILE, "r");
    if (!f) return 0;
    int count = 0;
    while (count < max_size)
    {
        struct CaseRecord temp;
        if (fscanf(f, "%d %49s %29s %d %19s %19s",
                   &temp.id, temp.caller, temp.type, &temp.priority, temp.status, temp.unit) != 6)
        {
            break;
        }
        arr[count] = temp;
        count++;
    }
    fclose(f);
    return count;
}

void save_all_cases(struct CaseRecord arr[], int n)
{
    FILE *f = fopen(CASES_FILE, "w");
    if (!f)
    {
        printf("Error: cannot open cases file for writing!\n");
        return;
    }
    for (int i = 0; i < n; i++)
    {
        fprintf(f, "%d %s %s %d %s %s\n",
                arr[i].id, arr[i].caller, arr[i].type,
                arr[i].priority, arr[i].status, arr[i].unit);
    }
    fclose(f);
}

int get_next_case_id()
{
    FILE *f = fopen(CASES_FILE, "r");
    if (!f) return 1;
    int maxid = 0;
    while (1)
    {
        int id;
        if (fscanf(f, "%d", &id) != 1) break;
        if (id > maxid) maxid = id;
        int c;
        while ((c = fgetc(f)) != '\n' && c != EOF) { }
    }
    fclose(f);
    return maxid + 1;
}

void add_case()
{
    struct CaseRecord c;
    c.id = get_next_case_id();
    printf("\n*Add New Case*\n");
    printf("Auto Case ID: %d\n", c.id);
    printf("Enter caller name without spaces: ");
    if (scanf("%49s", c.caller) != 1)
    {
        while(getchar()!='\n');
        return;
    }
    while (getchar() != '\n');
    printf("Enter type without spaces: ");
    if (scanf("%29s", c.type) != 1)
    {
        while(getchar()!='\n');
        return;
    }
    while (getchar() != '\n');
    printf("Enter priority (1 = highest, 2, 3): ");
    if (scanf("%d", &c.priority) != 1)
    {
        while(getchar()!='\n');
        return;
    }
    while (getchar() != '\n');
    if (c.priority < 1) c.priority = 1;
    if (c.priority > 3) c.priority = 3;
    strcpy(c.status, "Pending");
    strcpy(c.unit, "None");
    append_case_to_file(c);
    printf("Case %d added.\n", c.id);
}

void sort_cases_by_priority(struct CaseRecord arr[], int n)
{
    for (int i = 0; i < n-1; i++)
    {
        for (int j = 0; j < n-1-i; j++)
        {
            if (arr[j].priority > arr[j+1].priority)
            {
                struct CaseRecord tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

void view_cases_priority()
{
    struct CaseRecord arr[MAX_CASES];
    int n = load_all_cases(arr, MAX_CASES);
    if (n == 0)
    {
        printf("\nNo cases found :( \n");
        return;
    }
    sort_cases_by_priority(arr, n);
    printf("\n*All cases (priority 1 highest)*\n");
    for (int i = 0; i < n; i++)
    {
        printf("ID:%d Caller:%s Type:%s Priority:%d Status:%s Unit:%s\n",
               arr[i].id, arr[i].caller, arr[i].type, arr[i].priority, arr[i].status, arr[i].unit);
    }
}

void search_case()
{
    printf("\n*Search Case by ID*\nEnter Case ID: ");
    int id;
    if (scanf("%d", &id) != 1)
    {
        while(getchar()!='\n');
        printf("Invalid.\n");
        return;
    }
    while (getchar() != '\n');
    struct CaseRecord arr[MAX_CASES];
    int n = load_all_cases(arr, MAX_CASES);
    int found = 0;
    for (int i = 0; i < n; i++)
    {
        if (arr[i].id == id)
        {
            printf("Found: ID:%d Caller:%s Type:%s Priority:%d Status:%s Unit:%s\n",
                   arr[i].id, arr[i].caller, arr[i].type, arr[i].priority, arr[i].status, arr[i].unit);
            found = 1;
            break;
        }
    }
    if (!found) printf("Case %d not found.\n", id);
}

void edit_case()
{
    printf("\n*Edit Case*\nEnter Case ID to edit: ");
    int id;
    if (scanf("%d", &id) != 1)
    {
        while(getchar()!='\n');
        printf("Invalid.\n");
        return;
    }
    while (getchar() != '\n');
    struct CaseRecord arr[MAX_CASES];
    int n = load_all_cases(arr, MAX_CASES);
    int idx = -1;
    for (int i = 0; i < n; i++) if (arr[i].id == id)
        {
            idx = i;
            break;
        }
    if (idx == -1)
    {
        printf("Case not found.\n");
        return;
    }

    printf("Current: Caller:%s Type:%s Priority:%d Status:%s Unit:%s\n",
           arr[idx].caller, arr[idx].type, arr[idx].priority, arr[idx].status, arr[idx].unit);

    printf("Edit: 1=Caller 2=Type 3=Priority (enter number): ");
    int ch;
    if (scanf("%d", &ch) != 1)
    {
        while(getchar()!='\n');
        printf("Invalid.\n");
        return;
    }
    while (getchar() != '\n');
    if (ch == 1)
    {
        printf("Enter new caller without spaces: ");
        if (scanf("%49s", arr[idx].caller) != 1)
        {
            while(getchar()!='\n');
            return;
        }
        while (getchar() != '\n');
    }
    else if (ch == 2)
    {
        printf("Enter new type without spaces: ");
        if (scanf("%29s", arr[idx].type) != 1)
        {
            while(getchar()!='\n');
            return;
        }
        while (getchar() != '\n');
    }
    else if (ch == 3)
    {
        printf("Enter new priority (1 highest): ");
        if (scanf("%d", &arr[idx].priority) != 1)
        {
            while(getchar()!='\n');
            return;
        }
        while (getchar() != '\n');
        if (arr[idx].priority < 1) arr[idx].priority = 1;
        if (arr[idx].priority > 3) arr[idx].priority = 3;
    }
    else
    {
        printf("Invalid option.\n");
        return;
    }

    save_all_cases(arr, n);
    printf("Case %d updated.\n", id);
}

void delete_case()
{
    printf("\n*Delete Case*\nEnter Case ID to delete: ");
    int id;
    if (scanf("%d", &id) != 1)
    {
        while(getchar()!='\n');
        printf("Invalid.\n");
        return;
    }
    while (getchar() != '\n');
    struct CaseRecord arr[MAX_CASES];
    int n = load_all_cases(arr, MAX_CASES);
    int idx = -1;
    for (int i = 0; i < n; i++) if (arr[i].id == id)
        {
            idx = i;
            break;
        }
    if (idx == -1)
    {
        printf("Case not found.\n");
        return;
    }

    if (strcmp(arr[idx].status, "Assigned") == 0 && strcmp(arr[idx].unit, "None") != 0)
    {
        struct Units u = readUnits();
        if (strcmp(arr[idx].unit, "Police") == 0) u.police++;
        else if (strcmp(arr[idx].unit, "Fire") == 0) u.fire++;
        else if (strcmp(arr[idx].unit, "Ambulance") == 0) u.ambulance++;
        write_units(u);
    }

    for (int i = idx; i < n-1; i++) arr[i] = arr[i+1];
    n--;
    save_all_cases(arr, n);
    printf("Case %d deleted.\n", id);
}

void dispatch_unit(char *logged_by)
{
    struct CaseRecord arr[MAX_CASES];
    int n = load_all_cases(arr, MAX_CASES);
    if (n == 0)
    {
        printf("No cases to dispatch.\n");
        return;
    }
    printf("\n*Dispatch Unit*\nEnter Case ID: ");
    int id;
    if (scanf("%d", &id) != 1)
    {
        while(getchar()!='\n');
        printf("Invalid.\n");
        return;
    }
    while (getchar() != '\n');
    int idx = -1;
    for (int i = 0; i < n; i++) if (arr[i].id == id)
        {
            idx = i;
            break;
        }
    if (idx == -1)
    {
        printf("Case not found.\n");
        return;
    }
    if (strcmp(arr[idx].status, "Assigned") == 0)
    {
        printf("Already assigned to %s.\n", arr[idx].unit);
        return;
    }
    if (strcmp(arr[idx].status, "Resolved") == 0)
    {
        printf("Already resolved.\n");
        return;
    }

    printf("Choose unit (Police/Fire/Ambulance): ");
    char choice[20];
    if (scanf("%19s", choice) != 1)
    {
        while(getchar()!='\n');
        return;
    }
    while (getchar() != '\n');
    if (choice[0] == 'p' || choice[0] == 'P') strcpy(choice, "Police");
    else if (choice[0] == 'f' || choice[0] == 'F') strcpy(choice, "Fire");
    else if (choice[0] == 'a' || choice[0] == 'A') strcpy(choice, "Ambulance");
    else
    {
        printf("Invalid unit.\n");
        return;
    }

    struct Units u = readUnits();
    if (strcmp(choice, "Police") == 0)
    {
        if (u.police <= 0)
        {
            printf("No Police units available :( \n");
            return;
        }
        u.police--;
    }
    else if (strcmp(choice, "Fire") == 0)
    {
        if (u.fire <= 0)
        {
            printf("No Fire units available :( \n");
            return;
        }
        u.fire--;
    }
    else if (strcmp(choice, "Ambulance") == 0)
    {
        if (u.ambulance <= 0)
        {
            printf("No Ambulance units available :( \n");
            return;
        }
        u.ambulance--;
    }

    strcpy(arr[idx].unit, choice);
    strcpy(arr[idx].status, "Assigned");
    write_units(u);
    save_all_cases(arr, n);
    printf("Case %d assigned to %s by %s.\n", id, choice, logged_by);
}

void show_units()
{
    struct Units u = readUnits();
    printf("\n*Units Available*\n");
    printf("Police: %d\n", u.police);
    printf("Fire: %d\n", u.fire);
    printf("Ambulance: %d\n", u.ambulance);
}

void resolve_case()
{
    struct CaseRecord arr[MAX_CASES];
    int n = load_all_cases(arr, MAX_CASES);
    if (n == 0)
    {
        printf("No cases.\n");
        return;
    }
    printf("\n*Resolve Case*\nEnter Case ID: ");
    int id;
    if (scanf("%d", &id) != 1)
    {
        while(getchar()!='\n');
        printf("Invalid.\n");
        return;
    }
    while (getchar() != '\n');
    int idx = -1;
    for (int i = 0; i < n; i++)
        if (arr[i].id == id)
        {
            idx = i;
            break;
        }
    if (idx == -1)
    {
        printf("Case not found.\n");
        return;
    }
    if (strcmp(arr[idx].status, "Resolved") == 0)
    {
        printf("Already resolved.\n");
        return;
    }

    struct Units u = readUnits();
    if (strcmp(arr[idx].unit, "Police") == 0)
        u.police++;
    else if (strcmp(arr[idx].unit, "Fire") == 0)
        u.fire++;
    else if (strcmp(arr[idx].unit, "Ambulance") == 0)
        u.ambulance++;

    strcpy(arr[idx].status, "Resolved");
    write_units(u);
    save_all_cases(arr, n);
    printf("Case %d marked Resolved. Unit released.\n", id);
}

void admin_menu(char *username);
void dispatcher_menu(char *username);

void admin_menu(char *username)
{
    int ch;
    printf("\n*ADMIN MENU*\n");
    printf("1.Add Case 2.View Cases 3.Dispatch 4.Resolve 5.Search 6.Edit 7.Delete 8.Show Units 9.Register dispatcher 10.Logout\n");
    printf("Choice: ");

    if (scanf("%d", &ch) != 1)
    {
        while(getchar()!='\n');
        printf("Invalid.\n");
        admin_menu(username);
        return;
    }
    while (getchar() != '\n');

    switch(ch)
    {
    case 1:
        add_case();
        break;
    case 2:
        view_cases_priority();
        break;
    case 3:
        dispatch_unit(username);
        break;
    case 4:
        resolve_case();
        break;
    case 5:
        search_case();
        break;
    case 6:
        edit_case();
        break;
    case 7:
        delete_case();
        break;
    case 8:
        show_units();
        break;
    case 9:
        registerDispatcher();
        break;
    case 10:
        printf("Logging out.\n");
        return;
    default:
        printf("Invalid choice.\n");
    }

    admin_menu(username);
}

void dispatcher_menu(char *username)
{
    int ch;
    printf("\n*Dispatcher MENU*\n");
    printf("1.Add Case 2.View Cases 3.Dispatch 4.Resolve 5.Search 6.Show Units 7.Logout\n");
    printf("Choice: ");

    if (scanf("%d", &ch) != 1)
    {
        while(getchar()!='\n');
        printf("Invalid.\n");
        dispatcher_menu(username);
        return;
    }
    while (getchar() != '\n');

    switch(ch)
    {
    case 1:
        add_case();
        break;
    case 2:
        view_cases_priority();
        break;
    case 3:
        dispatch_unit(username);
        break;
    case 4:
        resolve_case();
        break;
    case 5:
        search_case();
        break;
    case 6:
        show_units();
        break;
    case 7:
        printf("Logging out.\n");
        return;
    default:
        printf("Invalid choice.\n");
    }

    dispatcher_menu(username);
}

void main_menu();

void main_menu()
{
    int choice;
    printf("\n*911 Emergency Dispatch System*\n");
    printf("1.Register dispatcher 2.Login 3.Show Units 4.Exit\n");
    printf("Enter choice: ");

    if (scanf("%d", &choice) != 1)
    {
        while(getchar()!='\n');
        printf("Invalid.\n");
        main_menu();
        return;
    }
    while (getchar() != '\n');

    switch(choice)
    {
    case 1:
        registerDispatcher();
        break;
    case 2:
    {
        char username[80], role[20];
        if (login(username, role))
        {
            printf("Welcome %s!!\n", username);
            if (strcmp(role, "admin") == 0)
                admin_menu(username);
            else
                dispatcher_menu(username);
        }
        else
        {
            printf("Login failed. Check credentials.\n");
        }
        break;
    }
    case 3:
        show_units();
        break;
    case 4:
        printf("Bye for now.\n");
        return;
    default:
        printf("Invalid choice.\n");
    }

    main_menu();
}

int main()
{
    create_units_file();
    main_menu();
    return 0;
}
