#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random, string, os, sys

# Creamos un directorio temporario donde escribir todas las salidas.
TMP_DIR = '/tmp/tmp-' + ''.join(random.choice(string.lowercase) for i in range(10)) + '/'
os.system('mkdir ' + TMP_DIR)
print 'El output sera grabado en', TMP_DIR

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
cmd += 'cd HTML2TXT-0.3/; wget -qO- "%s" | ./HTML2TXT -o %s/HtmlToTxt.txt ; cd ..;' % (sys.argv[1], TMP_DIR)

# Separa en oraciones.
cmd += 'cd SeparaOraciones-0.3/; ./SeparaOraciones -i %s/HtmlToTxt.txt -o %s/TextoSeparado.txt ; cd ..;' % (TMP_DIR, TMP_DIR)

# Traduce palabras extranjeras.
cmd += 'cd Traductor-0.3/; ./Traductor -i %s/TextoSeparado.txt -o %s/TextoTraducido.txt ; cd ..;' % (TMP_DIR, TMP_DIR)

# Normaliza el texto.
cmd += 'cd Normalizador-0.3/; ./Normalizador -i %s/TextoTraducido.txt -o %s/SalidaNormalizador.txt  > /dev/null; cd ..;' % (TMP_DIR, TMP_DIR)

print 'Salida: %s/SalidaNormalizador.txt' % TMP_DIR

# Ejecutamos los comandos.
os.system(cmd)

