# Proyecto-Final-Programacion-2
...
Trabajo Final
Grupos: 2 estudiantes
Un Software de Reproducción de Música
Desarrolle un software interactivo en C que simule las funcionalidades avanzadas de
Spotify, incluyendo:
• Gestión dinámica de usuarios y perfiles
• Sistema de reproducción con anuncios
• Generación de estadísticas de uso
• Recomendaciones personalizadas

Estructuras de datos requeridas
• Árboles ABB para usuarios y artistas
• Listas ligadas para playlists, discos, etc.
• Colas para gestión de anuncios
• Pilas para historial de reproducción

Detalles de la implementación
El software deberá manejar sus usuarios. De un usuario se conoce:
• Correo electrónico
• Nombre
• País de origen
• Nickname
• Contraseña
• Plan (free/premium)
Se debe realizar mantenimiento (CRUD) de usuarios. Los usuarios se guardarán en un
ABB.

Además, los usuarios tendrán un historial de reproducción. De una reproducción se
conoce la canción y la fecha. Para la fecha se deberá definir un formato que permita el
ingreso de datos válidos.

Los usuarios podrán tener cero, una o muchas playlists personales que se guardarán en
listas ligadas. Un playlist tiene un nombre y canciones que pueden ser de varios artistas.

Gestión de artistas, discos y canciones
De los artistas se conoce:
• Nombre
• Discos
De un disco se conoce:
• Nombre
• Fecha de lanzamiento
• Lista ligada de canciones
Un artista debe tener al menos un disco y un disco al menos una canción.
De las canciones se conoce:
• Artista
• Nombre
• Duración
• Archivo de origen
El archivo de origen puede ser un archivo MP3, una URL u otra alternativa.
Las canciones deben implementar CRUD, pero solo podrán eliminarse si no pertenecen
a ningún playlist.

Sistema de anuncios
Los usuarios con plan free deberán escuchar anuncios. Cada tres canciones se insertará
un anuncio.
De los anuncios se conoce:
• Contenido del anuncio (texto, MP3 u otra alternativa)
• Anunciante
Los anuncios se gestionarán mediante una cola de reproducción.
Los usuarios premium escucharán sus reproducciones sin interrupciones.
Plan Premium
Los usuarios podrán:
• Comprar el plan premium
• Renovar el plan premium
Al comprar un plan se indicará:
• Validez
• Valor

Amigos y recomendaciones
Los usuarios podrán tener amigos mediante una lista ligada de amigos.
El software podrá recomendar a un usuario las canciones que un amigo haya escuchado.

Historial de reproducción
El software permitirá conocer las canciones que un usuario ha escuchado desde la última
hasta la primera reproducción.
Se mostrarán los títulos de las canciones.

Estadísticas y reportes
Se generarán reportes en archivos .txt para:
• Top 5 canciones más escuchadas
• Artistas preferidos por usuario
• Tiempo total de reproducción
• Número de anuncios mostrados (solo usuarios free)

Entregables
• Código fuente
• Diagrama de estructuras de datos
• Datos de prueba (archivos .txt con canciones y usuarios de ejemplo)

Consideraciones
Si bien se han colocado requisitos mínimos respecto a las estructuras de datos dinámicas
a emplear, este trabajo deberá ser bien diseñado.
Procure crear estructuras sólidas que faciliten la programación y el uso del software.
Antes de escribir código, diseñe eficazmente.

Evaluación
• Diseño de estructuras de datos: 25%
• Implementación de estructuras de datos: 25%
• Interfaz de usuario: 15%
• Documentación: 5%
• Sustentación: 30%

Reto de equipos
Cada equipo presentará su solución en 30 minutos.
La docente realizará consultas a cualquier integrante del equipo, quien deberá
responder.
Cada miembro del equipo calificará a cada compañero.
Incentivo
La mejor solución obtendrá 10 puntos para el examen final. 