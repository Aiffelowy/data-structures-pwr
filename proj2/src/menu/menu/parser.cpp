#include "parser.hpp"
#include "menu.hpp"
#include "builtin_actions.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>



namespace MenuN::Parser {

Parser::Parser(std::string filename) : filename(filename) {}


std::string Parser::remove_spaces(std::string str) {
  str.erase(
    std::remove_if(
    str.begin(),
    str.end(),
    [l = std::locale()](auto ch){ return std::isspace(ch, l); }
    ), str.end());
  
  return str;
}

std::string Parser::remove_spaces_qouted(std::string str) {
  std::istringstream is_str(str);
  std::string substr;

  std::string new_string = "";
  bool del_spaces = true;

  while(std::getline(is_str, substr, '\"')) {
    if(del_spaces) {
      substr = remove_spaces(substr);
    }

    new_string += substr;
    del_spaces = !del_spaces;
  }

  return new_string;
}

void build_in(CaCtx& cactx) {
  CA ca("Back");
  ca.so_file = "builtin";
  cactx.avail_types.push_back(ca);
  ca.name = "Quit";
  cactx.avail_types.push_back(ca);
  ca.name = "ChangeMenu";
  ca.args= {"label", "destination"};
  cactx.avail_types.push_back(ca);
}

Result::Result<Menu, CfgError> Parser::to_menu() {
  Menu menu;

  std::ifstream file(filename);

  std::string line;
  
  std::vector<MnCtx> menu_ctxs;
  CaCtx cactx;

  build_in(cactx);

  while(std::getline(file, line)) {
    std::string key;
    std::string ctx;
    line = remove_spaces_qouted(line);
    std::istringstream is_line(line);

    if(std::getline(is_line, ctx, ':')) {
      MnCtx mctx;
      std::getline(is_line, key);
      int braces = 1;
      while(std::getline(file, line)) {
        line = remove_spaces_qouted(line);
        is_line = std::istringstream(line);
        while(std::getline(is_line, key, ':')) {
          std::string right_side = "";
          std::getline(is_line, right_side);
          if(key == "{") { braces++; continue; };
          if(key == "}") { braces--; continue; };
          if(right_side == "{") braces++;
          if(right_side == "}") braces--;

          //handle shit
          if(ctx == "custom_actions") {
            CA ca;
            ca.name = key;
            std::istringstream is_right_side(right_side);
            std::getline(is_right_side, ca.so_file, ',');
            std::string temp;
            while(std::getline(is_right_side, temp, ',')) {
              ca.args.push_back(temp);
            }

            menu.open_shared_libraries.push_back(slloader::SLLoader<Actions::Action>(ca.so_file));
            menu.open_shared_libraries.back().open_lib().unwrap();
            cactx.avail_types.push_back(ca);

          } else if(ctx == "menu") {

            if(key == "id") {
              mctx.id = right_side;
            } else if(key == "title") {
              mctx.title = right_side;
            } else if(key == "subtitle") {
              mctx.subtitle = right_side;
            } else if(key == "actions") {
              while(std::getline(file, line)) {
                line = remove_spaces_qouted(line);
                is_line = std::istringstream(line);
                std::string action_key;

                if(line.find(':') == std::string::npos) {
                  std::getline(is_line, action_key);
                  if(action_key == "}") {
                    braces--;
                    break;
                  }
                  CA type = cactx.get_type(action_key).unwrap();
                  if(type.so_file == "builtin") {
                    if(type.name == "Back") {
                      mctx.actions.push_back(std::make_shared<Actions::Back>(Actions::Back()));
                    }
                    else if(type.name == "Quit") {
                      mctx.actions.push_back(std::make_shared<Actions::Quit>(Actions::Quit()));
                    }
                    continue;
                  }
                  //create action
                  for(auto& shared_lib : menu.open_shared_libraries) {
                    if(shared_lib.get_path() == type.so_file)
                      mctx.actions.push_back(shared_lib.get_instance());
                  }
                  continue;
                }
                

                while(std::getline(is_line, action_key, ':')) {
                  std::string action_right_side = "";
                  std::getline(is_line, action_right_side);

                  std::vector<std::string> args;
                  std::istringstream is_right_side(action_right_side);
                  std::string arg;
                  while(std::getline(is_right_side, arg, ',')) {
                    args.push_back(arg);
                  }
                  
                  auto temp = cactx.get_type(action_key);
                  bool f = false;
                  CA type = matcho(temp, {
                    case Some: return temp.unwrap();
                    case None: f = true;
                  });
                  if(f)
                    return Result::Err(CfgError::UnknownKeyword);
                  

                  if(type.so_file == "builtin") {
                    if(type.name == "ChangeMenu") {
                      mctx.actions.push_back(std::make_shared<Actions::ChangeMenu>(Actions::ChangeMenu(args[0], args[1])));
                    }
                    continue;
                  }

                  std::shared_ptr<Actions::Action> p;

                  //create action with arguments;
                  for(auto& shared_lib : menu.open_shared_libraries) {
                    if(shared_lib.get_path() != type.so_file)
                      continue;

                    switch(type.get_arg_size()) {
                      case 1: {
                        p = shared_lib.get_instance(args[0]);
                        break;
                      }
                      case 2: {
                        p = shared_lib.get_instance(args[0], args[1]);
                        break;
                      }
                      case 3: {
                        p = shared_lib.get_instance(args[0], args[1], args[2]);
                        break;
                      }
                     default:
                        break;
                    }
                  }
                  mctx.actions.push_back(p);
                }
              }
            } else {
              return Result::Err(CfgError::UnknownKeyword);
            }
          } else {
            return Result::Err(CfgError::UnknownKeyword);
          }
        }
          
        if(braces == 0) break;
      }
      if(ctx == "menu") {
        menu.add_menu(mctx.into_submenu().unwrap())
        .unwrap();
      }
    }
  }

  return Result::Ok(menu);
}

}







