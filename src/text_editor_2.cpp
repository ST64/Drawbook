#include <cstdio>
#include <inttypes.h>
#include <cstdlib>
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Window.H>
#include <FL/fl_ask.H>
#include <FL/fl_file_chooser.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Text_Display.H>
using namespace std;
void style_parse(const char *text,char *style,int length);
Fl_Window *window = new Fl_Window(640, 480, "Untitled");
Fl_Menu_Bar *menubar = new Fl_Menu_Bar(0, 0, 640, 30);
Fl_Text_Editor *input = new Fl_Text_Editor(0, 30, 640, 430);
Fl_Window *replace_dlg = new Fl_Window(300, 105, "Replace");
Fl_Input *replace_find = new Fl_Input(70, 10, 210, 25, "Find:");
Fl_Input *replace_with = new Fl_Input(70, 40, 210, 25, "Replace:");
Fl_Button *replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
Fl_Return_Button *replace_next = new Fl_Return_Button(105, 70, 120, 25, "Replace Next");
Fl_Button *replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");;
Fl_Text_Buffer *textbuf = new Fl_Text_Buffer(4096);
Fl_Text_Buffer *stylebuf = new Fl_Text_Buffer(4096);

int changed = 0;
char filename[128] = "";
char search[128] = "";

void set_changed(int);
void save_cb(void);
void saveas_cb(void);
void find2_cb(void);

const char *code_keywords[] = {
                     "and",
                     "and_eq",
                     "asm",
                     "bitand",
                     "bitor",
                     "break",
                     "case",
                     "catch",
                     "compl",
                     "continue",
                     "default",
                     "delete",
                     "do",
                     "else",
                     "false",
                     "for",
                     "goto",
                     "if",
                     "new",
                     "not",
                     "not_eq",
                     "operator",
                     "or",
                     "or_eq",
                     "return",
                     "switch",
                     "template",
                     "this",
                     "throw",
                     "true",
                     "try",
                     "using",
                     "while",
                     "xor",
                     "xor_eq"
                   };
const char *code_types[] = {
                     "auto",
                     "bool",
                     "char",
                     "class",
                     "const",
                     "const_cast",
                     "double",
                     "dynamic_cast",
                     "enum",
                     "explicit",
                     "extern",
                     "float",
                     "friend",
                     "inline",
                     "int",
                     "long",
                     "mutable",
                     "namespace",
                     "private",
                     "protected",
                     "public",
                     "register",
                     "short",
                     "signed",
                     "sizeof",
                     "static",
                     "static_cast",
                     "struct",
                     "template",
                     "typedef",
                     "typename",
                     "union",
                     "unsigned",
                     "virtual",
                     "void",
                     "volatile"
                   };
int check_save(void) {
  return !changed;
}

void load_file(char *newfile) {
  FILE *fp;
  char buffer[8192];
  int nbytes;
  int pos;

  textbuf->text("");

  fp = fopen(newfile, "r");
  if (fp != NULL) {

    strcpy(filename, newfile);
    pos = 0;

    while ((nbytes = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
      textbuf->replace(pos, pos+nbytes, buffer);
      pos += nbytes;
    }

    fclose(fp);
    input->insert_position(0);
    set_changed(0);
  }
}
int compare_keywords(const void *a,const void *b) {
  return (strcmp(*((const char **)a), *((const char **)b)));
}
void save_file(char *newfile) {
  FILE *fp;

  fp = fopen(newfile, "w");
  if (fp != NULL) {

    strcpy(filename, newfile);

    if (fwrite(textbuf->text(), 1, strlen(textbuf->text()), fp) < 1) {
      fl_alert("Unable to write file!");
      fclose(fp);
      return;
    }

    fclose(fp);
    changed = 1;
    set_changed(0);
  }
}

void set_changed(int c) {
  if (c != changed) {
    char title[1024];
    char *slash;

    changed = c;

    if (filename[0] == '\0') strcpy(title, "Untitled");
    else {
      slash = strrchr(filename, '/');
      if (slash == NULL) slash = strrchr(filename, '\\');

      if (slash != NULL) strcpy(title, slash + 1);
      else strcpy(title, filename);
    }

    if (changed) strcat(title, " (modified)");

    window->label(title);
  }
}
void changed_cb(int, int nInserted, int nDeleted,int, const char*, void* v) {
  set_changed(1);
}
void copy_cb(void) {
  Fl_Text_Editor::kf_copy(0, input);
}
void cut_cb(void) {
  Fl_Text_Editor::kf_cut(0, input);
}
void delete_cb(void) {
  Fl_Text_Editor::kf_cut(0, input);
}
void find_cb(void) {
  const char *val;

  val = fl_input("Search String:", search);
  if (val != NULL) {

    strcpy(search, val);
    find2_cb();
  }
}

void find2_cb(void) {
  const char *val, *found;
  int pos;

  if (search[0] == '\0') {

    find_cb();
    return;
  }
  pos = input->insert_position();

  if (found != NULL) {

    textbuf->select(pos,pos+strlen(search));
 input->insert_position(pos+strlen(search));
 input->show_insert_position();
  }
  else fl_alert("No occurrences of \'%s\' found!", search);
}

void new_cb(Fl_Widget *,void *) {
 filename[0] = '\0';
 textbuf->select(0, textbuf->length());
 textbuf->remove_selection();
 changed = 0;
 textbuf->call_modify_callbacks();
}

void open_cb(void) {
  char *newfile;

  if (changed)
    if (!check_save()) return;

  newfile = fl_file_chooser("Open File?", "*", filename);
  if (newfile != NULL) load_file(newfile);
}

void paste_cb(void) {
  Fl::paste(*input);
}

void quit_cb(void) {
  if (changed)
    if (!check_save())
      return;

  window->hide();
}

void replace_cb(void) {
  replace_dlg->show();
}

void replace2_cb() {
  const char *find, *val, *found;
  int pos;

  find = replace_find->value();
  if (find[0] == '\0') {

    replace_dlg->show();
    return;
  }

  replace_dlg->hide();

  pos = input->insert_position();

  if (found != NULL) {

    textbuf->replace(pos, pos + strlen(find), replace_with->value());
 input->insert_position(pos+strlen(replace_with->value()));
 input->show_insert_position();
  }
  else fl_alert("No occurrences of \'%s\' found!", find);
}

void replall_cb() {
  const char *find, *val, *found;
  int pos;
  int times;

  find = replace_find->value();
  if (find[0] == '\0') {

    replace_dlg->show();
    return;
  }

  replace_dlg->hide();

  input->insert_position(0);
  times = 0;


  do {
    pos = input->insert_position();
    found = strstr(val, find);

    if (found != NULL) {

      times ++;
      textbuf->replace(pos, pos + strlen(find), replace_with->value());
     input->insert_position(pos+strlen(replace_with->value()));
     input->show_insert_position();
    }
  } while (found != NULL);

  if (times > 0){fl_message("Replaced %d occurrences.", times);}
  else{fl_alert("No occurrences of \'%s\' found!", find);}
}

void replcan_cb() {
  replace_dlg->hide();
}

void save_cb(void) {
  if (filename[0] == '\0') {

    saveas_cb();
    return;
  }
  else save_file(filename);
}

void saveas_cb(void) {
  char *newfile;

  newfile = fl_file_chooser("Save File As?", "*", filename);
  if (newfile != NULL) save_file(newfile);
}

void undo_cb(void) {
  textbuf->undo();
}
Fl_Text_Display::Style_Table_Entry styletable[] = {
  { FL_BLACK, FL_COURIER, FL_NORMAL_SIZE },
  { FL_DARK_GREEN, FL_COURIER_ITALIC, FL_NORMAL_SIZE },
  { FL_DARK_GREEN, FL_COURIER_ITALIC, FL_NORMAL_SIZE },
  { FL_BLUE, FL_COURIER, FL_NORMAL_SIZE },
  { FL_DARK_RED, FL_COURIER, FL_NORMAL_SIZE },
  { FL_DARK_RED, FL_COURIER_BOLD, FL_NORMAL_SIZE },
  { FL_BLUE, FL_COURIER_BOLD, FL_NORMAL_SIZE }
};
Fl_Menu_Item menuitems[] = {
  { "&File", 0, 0, 0, FL_SUBMENU },
    { "&New", 0, (Fl_Callback *)new_cb },
    { "&Open...", 0, (Fl_Callback *)open_cb, 0, FL_MENU_DIVIDER },
    { "&Save", 0, (Fl_Callback *)save_cb },
    { "Save &As...", 0, (Fl_Callback *)saveas_cb, 0, FL_MENU_DIVIDER },
    { "&Quit", 0, (Fl_Callback *)quit_cb },
    { 0 },

  { "&Edit", 0, 0, 0, FL_SUBMENU },
    { "&Undo", 0, (Fl_Callback *)undo_cb, 0, FL_MENU_DIVIDER },
    { "Cu&t", 0, (Fl_Callback *)cut_cb },
    { "&Copy", 0, (Fl_Callback *)copy_cb },
    { "&Paste", 0, (Fl_Callback *)paste_cb },
    { "&Delete", 0, (Fl_Callback *)delete_cb },
    { 0 },

  { "&Search", 0, 0, 0, FL_SUBMENU },
    { "&Find...", 0, (Fl_Callback *)find_cb },
    { "F&ind Again", 0, (Fl_Callback *)find2_cb },
    { "&Replace...", 0, (Fl_Callback *)replace_cb },
    { "Re&place Again", 0, (Fl_Callback *)replace2_cb },
    { 0 },

  { 0 }
};
void style_update(int pos,int nInserted,int nDeleted,int nRestyled,const char* deletedText,void *cbArg) {
  int start,
       end;
  char last,
        *style,
        *text;

  if (nInserted == 0 && nDeleted == 0) {
    stylebuf->unselect();
    return;
  }

  if (nInserted > 0) {

    style = new char[nInserted + 1];
    memset(style, 'A', nInserted);
    style[nInserted] = '\0';
    stylebuf->replace(pos, pos + nDeleted, style);
    delete[] style;
  } else {

    stylebuf->remove(pos, pos + nDeleted);
  }


  stylebuf->select(pos, pos + nInserted - nDeleted);





  start = textbuf->line_start(pos);
  end = textbuf->line_end(pos + nInserted - nDeleted);
  text = textbuf->text_range(start, end);
  style = stylebuf->text_range(start, end);
  last = style[end - start - 1];
  style_parse(text, style, end - start);
  stylebuf->replace(start, end, style);
  ((Fl_Text_Editor *)cbArg)->redisplay_range(start, end);
  if (last != style[end - start - 1]) {


    free(text);
    free(style);
    end = textbuf->length();
    text = textbuf->text_range(start, end);
    style = stylebuf->text_range(start, end);
    style_parse(text, style, end - start);
    stylebuf->replace(start, end, style);
    ((Fl_Text_Editor *)cbArg)->redisplay_range(start, end);
  }
  free(text);
  free(style);
}
void style_unfinished_cb(int, void*){}
int main(int argc, char **argv) {
    menubar->menu(menuitems);
    input->textfont(FL_COURIER);
 input->buffer(textbuf);
 window->resizable(input);
 window->callback((Fl_Callback *)quit_cb);
 textbuf->add_modify_callback(style_update,input);
 textbuf->add_modify_callback(changed_cb,NULL);
 input->linenumber_width(15);
    replace_find->align(FL_ALIGN_LEFT);
    replace_with->align(FL_ALIGN_LEFT);
    replace_all->callback((Fl_Callback *)replall_cb);
    replace_next->callback((Fl_Callback *)replace2_cb);
    replace_cancel->callback((Fl_Callback *)replcan_cb);
 input->highlight_data(stylebuf,styletable,sizeof(styletable)/sizeof(styletable[0]),'A',style_unfinished_cb,0);
  replace_dlg->end();
  replace_dlg->set_modal();

  window->show();

  if (argc > 1) load_file(argv[1]);
  return Fl::run();
}
void style_parse(const char *text,char *style,int length) {
  char current;
  int col;
  int last;
  char buf[255],
              *bufptr;
  const char *temp;
  for (current = *style, col = 0, last = 0; length > 0; length --, text ++) {
    if (current == 'A') {

      if (col == 0 && *text == '#') {

        current = 'E';
      } else if (strncmp(text, "//", 2) == 0) {
        current = 'B';
      } else if (strncmp(text, "/*", 2) == 0) {
        current = 'C';
      } else if (strncmp(text, "\\\"", 2) == 0) {

        *style++ = current;
        *style++ = current;
        text ++;
        length --;
        col += 2;
        continue;
      } else if (*text == '\"') {
        current = 'D';
      } else if (!last && islower(*text)) {

        for (temp = text, bufptr = buf;
             islower(*temp) && bufptr < (buf + sizeof(buf) - 1);
             *bufptr++ = *temp++);
        if (!islower(*temp)) {
          *bufptr = '\0';
          bufptr = buf;
          if (bsearch(&bufptr, code_types,
                      sizeof(code_types) / sizeof(code_types[0]),
                      sizeof(code_types[0]), compare_keywords)) {
            while (text < temp) {
              *style++ = 'F';
              text ++;
              length --;
              col ++;
            }
            text --;
            length ++;
            last = 1;
            continue;
          } else if (bsearch(&bufptr, code_keywords,
                             sizeof(code_keywords) / sizeof(code_keywords[0]),
                             sizeof(code_keywords[0]), compare_keywords)) {
            while (text < temp) {
              *style++ = 'G';
              text ++;
              length --;
              col ++;
            }
            text --;
            length ++;
            last = 1;
            continue;
          }
        }
      }
    } else if (current == 'C' && strncmp(text, "*/", 2) == 0) {

       *style++ = current;
       *style++ = current;
      text ++;
      length --;
      current = 'A';
      col += 2;
      continue;
    } else if (current == 'D') {

      if (strncmp(text, "\\\"", 2) == 0) {

         *style++ = current;
         *style++ = current;
        text ++;
        length --;
        col += 2;
        continue;
      } else if (*text == '\"') {

         *style++ = current;
        col ++;
        current = 'A';
        continue;
      }
    }

    if (current == 'A' && (*text == '{' || *text == '}')) *style++ = 'G';
    else *style++ = current;
    col ++;
    last = isalnum(*text) || *text == '.';
    if (*text == '\n') {

      col = 0;
      if (current == 'B' || current == 'E') current = 'A';
    }
  }
}
