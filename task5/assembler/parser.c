#include <stdio.h>
#include <stdlib.h>
#include "fibon.h"


int main(int argc, char *argv[]) {
    int index;
    char* file_name=*++argv;
    if (argc == 1) {
        fprintf(stderr, "Error: missing output file path.\n");
        return 1;
    }
    if (argc > 2) {
        fprintf(stderr, "Error : too many arguments provided, expecting only file path\n");
        return 1;
    }
    printf("Provided file path is : %s\n", file_name);
    printf("Please enter the index of the desired fibonacci number, the entered number must be non-negative whole: \n");
    scanf("%d", &index);

    if (index < 0) {
        fprintf(stderr, "Error: Invalid input, Please enter a non-negative whole number.\n");
        return 1;
    }

    printf("Received number is %d.\n", index);

    if (index >= 94) {
        fprintf(stderr, "Warn: The 94th Fibonacci number is 12200160415121876738, which representation is larger than 8 bytes and might be displayed incorrectly on some platforms.\n");
    }

    Node* head = NULL;

    fibonacci_series_linked_list(index, &head);
    write_linked_list_to_file(head, file_name, index);

    return 0;
}


void append_node(Node** current_node, long long new_data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Error: Failed allocation memory for Node struct, exiting\n");
        exit(1);
    }
    new_node->value = new_data;
    new_node->next = NULL;

    if (*current_node == NULL) {
        *current_node = new_node;
        return;
    }

    new_node->next = *current_node;
    *current_node = new_node;
}

void parse_macros() {
}

void first_parse(const char *file_path) {
/*
1. אתחל DC0, IC0. 2. קרא את השורה הבאה מקובץ המקור. אם נגמר קובץ המקור, עבור ל-16. 3. האם השדה הראשון הוא סמל? אם לא, עבור ל-5. 4. הדלק דגל "יש הגדרת סמל".
5. האם זוהי הנחיה לאחסון נתונים, כלומר, האם הנחית data. או string.? אם לא, עבור ל- 8.
6. אם יש הגדרת סמל )תווית(, הכנס אותו לטבלת הסמלים עם סימון )סמל מסוג data(-
ערכו יהיה DC. )אם הסמל כבר נמצא בטבלה, יש להודיע על שגיאה(. 7. זהה את סוג הנתונים, קודד אותם בזיכרון, עדכן את מונה הנתונים DC בהתאם
לאורכם, חזור ל-2.
8. האם זו הנחיית extern. או הנחיית entry. ? אם לא, עבור ל-11.
9. האם זוהי הנחיית extern. ? אם כן, הכנס כל סמל )אחד או יותר( המופיע כאופרנד של ההנחיה לתוך טבלת הסמלים ללא ערך, עם סימון )סמל מסוג external(.
10. חזור ל-2.
אלגוריתם שלדי של האסמבלר
36￼
11. אם יש הגדרת סמל, הכנס אותו לטבלת הסמלים עם סימון )סמל מסוג code(. ערכו יהיה IC )אם הסמל כבר נמצא בטבלה יש להודיע על שגיאה(.
12. חפש את שם הפעולה בטבלת שמות הפעולות, ואם לא נמצא – הודע על שגיאה בשם ההוראה.
13. נתח את מבנה האופרנדים של ההוראה וחשב את L. בנה כעת את הקוד הבינארי של המילה הראשונה של הפקודה.
14. עדכן ICL + IC
15. חזור ל-2. 16. אם נמצאו שגיאות בקובץ המקור, עצור. 17. עדכן בטבלת הסמלים את ערכם של הסמלים מסוג data , ע"י הוספת הערך הסופי של IC
)ראו הסבר בהמשך(.
18. התחל מעבר שני
*/
}

void second_parse() {
/*
1. אתחלIC0 2. קרא את השורה הבאה מקובץ המקור. אם נגמר קובץ המקור, עבור ל- 10. 3. אם השדה הראשון הוא סמל, דלג עליו.
4. האם זוהי הנחייה data, .string, .extern.? אם כן, חזור ל-2. 5. האם זוהי הנחיה entry.? אם לא, עבור ל-7.
6. סמן בטבלת הסמלים את הסמלים המתאימים כ-entry. חזור ל-2. 7. השלם את קידוד האופרנדים החל מהמילה השנייה בקוד הבינארי של ההוראה, בהתאם
לשיטת המיעון. אם אופרנד הוא סמל, מצא את המען בטבלת הסמלים. 8. עדכן ICIC + L
9. חזור ל-2. 10. אם נמצאו שגיאות במעבר שני, עצור. 11. צור ושמור את קבצי הפלט: קובץ קוד המכונה קובץ סמלים חיצוניים, וקובץ סמלים של
נקודות כניסה )ראו פרטים נוספים בהמשך(.
*/
}

void write_linked_list_to_file(Node* head, const char* filename, int size) {
//    FILE* file = fopen(filename, "w");
//    if (file == NULL) {
//        fprintf(stderr, "Error: failed opening file %s\n", filename);
//        exit(1);
//    }
//    fprintf(file, "Printing the first %d numbers of the fibonacci sequence in a reverse order : \n", size);
//    fprintf(file, "___________________________\n");
//    int i;
//    Node* temp = head;
//    for (i = 0; i <= size; i++) {
//        printf("%dth fibonacci number is : %lld\n", size-i, temp->value);
//        fprintf(file, "%lld\n", temp->value);
//        temp = temp->next;
//    }

    fclose(file);
}