# ABOUT
- This repository is created for the CSE232 Spring 2025 Term Project.
- Please add your info below!
- This repository has 3 collaborators:
  | ID | Username | Real Name |
  |----------|----------|----------|
  | 20230702055 | ceviert  | Ertuğrul ÇEVİK |
  | 20230702066 | PoyrazErdem | Poyraz Erdem KOYUNCU |
  | 20230702039 | Genoxon | Alp Hasan ŞİMŞEK |
  | *** | mhfuzun | Muhammet Furkan UZUN |
  | your id | your username | your real name |
  | your id | your username | your real name |

# HOW TO USE THIS REPO:
## Firstly:
Download the library and install it using  
```bash
sudo apt-get install libncurses-dev 
```

## Clean:
```bash
make clean
```

## Build:
Standart build:
```bash
make all
```

if you want to build with DEBUG interface:
```bash
make all DEBUG=1
```

## Run:
```bash
make run
```

## AND:
```bash
make clean && make all && make run
```

# HOW TO (new to github)
## Clone
- Download and install [Git-BASH](https://git-scm.com/downloads).
- Launch Git-BASH and clone the repository:
  ```bash
  git clone https://github.com/ceviert/CSE232_Project.git
  ```
## Pull
- Before making any edits to the project, make sure you have the latest version of the code:
  ```bash
  git pull
  ```
  - Make sure that you are in the same directory as the project.

## Add the files to be tracked
- If you've created new files to be added, make sure to track them:
  ```bash
  git add .
  ```
  - "." means all.
## Commit & Push
- After your edits, commit your changes with easy-to-understand commit names (ex. "made changes to insertion functions" etc.):
  ```bash
  git commit -m "<your comments here>"
  ```
- Then you can push your code to the repository:
  ```bash
  git push
  ```
