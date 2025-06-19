#include "../colorization.h"
#include "../utils.h"

void pre_login_menu() {
  printf("\n%s=========================================\n", COLOR_CYAN);
  printf("         💰 Welcome to CBank System\n");
  printf("=========================================%s\n\n", COLOR_RESET);

  printf("%s👉 Select an option:%s\n\n", COLOR_GREEN, COLOR_RESET);

  printf("%s1%s - 🔐 Login to my account\n", COLOR_YELLOW, COLOR_RESET);
  printf("%s2%s - 📝 Register a new account\n", COLOR_YELLOW, COLOR_RESET);
  printf("%s3%s - 🚪 Quit system\n", COLOR_YELLOW, COLOR_RESET);

  printf("\n%s-----------------------------------------%s\n", COLOR_CYAN, COLOR_RESET);
}

void post_login_menu() {
  printf("\n%s=========================================\n", COLOR_CYAN);
  printf("          🏦 CBank - Account Menu\n");
  printf("=========================================%s\n\n", COLOR_RESET);

  printf("%s👉 Select an option:%s\n\n", COLOR_GREEN, COLOR_RESET);

  printf("%s1%s - 💰 Check balance\n", COLOR_YELLOW, COLOR_RESET);
  printf("%s2%s - ➕ Make a deposit\n", COLOR_YELLOW, COLOR_RESET);
  printf("%s3%s - ➖ Make a withdrawal\n", COLOR_YELLOW, COLOR_RESET);
  printf("%s4%s - 📜 View transaction history\n", COLOR_YELLOW, COLOR_RESET);
  printf("%s5%s - 📄 ️ View account info\n", COLOR_YELLOW, COLOR_RESET);
  printf("%s6%s - ✏️  Update account info\n", COLOR_YELLOW, COLOR_RESET);
  printf("%s7%s - 🚪 Logout\n", COLOR_YELLOW, COLOR_RESET);

  printf("\n%s-----------------------------------------%s\n", COLOR_CYAN, COLOR_RESET);
}

int get_opt_input(int min_opt, int max_opt) {
  char action_opt[10];
  printf("\n%s👉 Enter your choice:%s ", COLOR_GREEN, COLOR_RESET);
  fgets(action_opt, sizeof(action_opt), stdin);
  terminate_str_by_newline(action_opt);
  return validate_choice(action_opt, min_opt, max_opt);
}