#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void log_movement(const char *log_file, const char *file_ext, const char *dest) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    FILE *f = fopen(log_file, "a");
    if (f) {
        fprintf(f, "%04d-%02d-%02d %02d:%02d:%02d: Moved *.%s to %s\n",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec, file_ext, dest);
        fclose(f);
    } else {
        fprintf(stderr, "Failed to write to log file: %s\n", log_file);
    }
}

int confirm_action() {
    char buffer[128];
    FILE *p = popen("osascript -e 'display dialog \"Do you want to clear the downloads_backup and proceed?\" buttons {\"Yes\", \"No\"} default button \"No\"'", "r");
    if (!p) return 0;
    
    fgets(buffer, sizeof(buffer), p);
    pclose(p);
    
    if (strstr(buffer, "Yes")) {
        return 1;  // User confirmed the action
    } else {
        return 0;  // User denied the action or some error occurred
    }
}


int main() {
    if (!confirm_action()) {
        printf("Operation cancelled by the user.\n");
        return 0;
    }
    char *home = getenv("HOME");
    const char *log_file_path = "/tmp/sort_downloads_log.txt";  // Change this to your preferred location

    if (!home) {
        fprintf(stderr, "Error: Could not get HOME directory.\n");
        return 1;
    }

    char cmd[1024];

    // Clear the existing backup directory
    snprintf(cmd, sizeof(cmd), "rm -rf %s/downloads_backup", home);
    system(cmd);

    // Create the directories
    snprintf(cmd, sizeof(cmd), "mkdir -p %s/downloads_backup/pdfs", home);
    system(cmd);

    snprintf(cmd, sizeof(cmd), "mkdir -p %s/downloads_backup/images", home);
    system(cmd);

    snprintf(cmd, sizeof(cmd), "mkdir -p %s/downloads_backup/misc", home);
    system(cmd);

    // Move PDFs
    snprintf(cmd, sizeof(cmd), "mv %s/Downloads/*.pdf %s/downloads_backup/pdfs 2>/dev/null", home, home);
    system(cmd);
    log_movement(log_file_path, "pdf", "pdfs");

    // Move Images
    const char *img_extensions[] = {"jpg", "jpeg", "png", "gif", "bmp"};
    int num_ext = sizeof(img_extensions) / sizeof(img_extensions[0]);
    for (int i = 0; i < num_ext; i++) {
        snprintf(cmd, sizeof(cmd), "mv %s/Downloads/*.%s %s/downloads_backup/images 2>/dev/null", home, img_extensions[i], home);
        system(cmd);
        log_movement(log_file_path, img_extensions[i], "images");
    }

    // Move xcarchive directories
    snprintf(cmd, sizeof(cmd), "mv %s/Downloads/*.xcarchive %s/downloads_backup/misc 2>/dev/null", home, home);
    system(cmd);
    log_movement(log_file_path, "xcarchive", "misc");

    // Move remaining files and directories to misc
    snprintf(cmd, sizeof(cmd), "find %s/Downloads/* -maxdepth 1 ! -path %s/downloads_backup -exec mv {} %s/downloads_backup/misc \\;", home, home, home);
    system(cmd);
    log_movement(log_file_path, "*", "misc");

    printf("Sorting complete.\n");

    return 0;
}
