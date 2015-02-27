#!/bin/bash

INPUT=""
OUTPUT=""

#if exists all_tmpl.cpp
#    rm all_tmpl.cpp
#fi exists all_tmpl.cp)

while getopts ":i:o:" opt; do
  case $opt in
    i)
      INPUT=$OPTARG
      ;;
    o)
      OUTPUT=$OPTARG
      ;;
    \?)
      echo "Invalid option: -$OPTARG  ./make_templates.sh -i . -o deploy" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument. -i input dir -o output dir" >&2
      exit 1
      ;;
  esac
done

# копируем файлик конфигурации в папку билда
cp $INPUT/deploy/config.json $OUTPUT

# сюда пишем все шаблоны
TEMPLATES=""
TEMPLATES="$TEMPLATES $INPUT/templates/_master.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/dashboard.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/login.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/catalog.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/shopcart.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/search.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/messages.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/messages-list.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/messages-compose.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/messages-read.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/messages-reply.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/calendar.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/reports.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/journals.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/service.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/news.tmpl"
TEMPLATES="$TEMPLATES $INPUT/templates/profile.tmpl"

#TEMPLATES="$TEMPLATES $INPUT/templates/head.tmpl"
#TEMPLATES="$TEMPLATES $INPUT/templates/header.tmpl"

# прожевываем шаблоны в срр-шник
cppcms_tmpl_cc $TEMPLATES -o $INPUT/all_tmpl.cpp

date

# собираем шаблоны в библиотеку
clang++ -shared -fPIC $INPUT/all_tmpl.cpp -std=c++11 -I "/usr/local/include" -L "/usr/local/lib"  -o $OUTPUT/libcpp_defskin.dylib -lcppcms -lbooster
