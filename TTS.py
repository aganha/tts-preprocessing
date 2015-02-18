#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright 2015 Universidad de Buenos Aires
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import random, string, os, sys

# Creamos un directorio temporario donde escribir todas las salidas.
TMP_DIR = os.getcwd() + '/tmp-' + ''.join(random.choice(string.lowercase) for i in range(10))
os.system('mkdir ' + TMP_DIR)

if len(sys.argv) == 1:
  print
  print "Front-End de un Sistema de Síntesis del Habla"
  print "Departamento de Computación - Facultad de Ciencias Exactas y Naturales - Universidad de Buenos Aires"
  print
  print "Se requiere la URL de la página de Wikipedia a sintetizar: "
  print
  print "Sintaxis: ./TTS.py URL" 
  print
  print "Ejemplo: ./TTS.py http://es.wikipedia.org/wiki/Universidad_de_Buenos_Aires"
  print
  sys.exit(1)

# Preparamos la secuencia de comandos a ejecutar.
cmd = ''

# Traemos la página de Wikipedia (el proxy es si estamos
# dentro de *.exp.dc.uba.ar)
cmd += 'export http_proxy="http://proxy.fcen.uba.ar:8080";'
cmd += 'cd HTML2TXT-0.2/; wget -qO- "%s" | ./HTML2TXT -o %s/HtmlToTxt.txt ; cd ..;' % (sys.argv[1], TMP_DIR)

# Separa en oraciones.
cmd += 'cd SeparaOraciones-0.2/; ./SeparaOraciones -i %s/HtmlToTxt.txt -o %s/TextoSeparado.txt ; cd ..;' % (TMP_DIR, TMP_DIR)

# Traduce palabras extranjeras.
cmd += 'cd Traductor-0.2/; ./Traductor -i %s/TextoSeparado.txt -o %s/TextoTraducido.txt ; cd ..;' % (TMP_DIR, TMP_DIR)

# Normaliza el texto.
cmd += 'cd Normalizador-0.2/; ./Normalizador -i %s/TextoTraducido.txt -o %s/SalidaNormalizador.txt  > /dev/null; cd ..;' % (TMP_DIR, TMP_DIR)

print 'Salida: %s/SalidaNormalizador.txt' % TMP_DIR

# Ejecutamos los comandos.
os.system(cmd)

