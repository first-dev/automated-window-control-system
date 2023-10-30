<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);
?>
<?php
if (isset($_GET['openWindow'])) {
  exec("sudo /home/moham/www/c-bin/control_window.o 1 2>&1");
}
if (isset($_GET['closeWindow'])) {
  exec("sudo /home/moham/www/c-bin/control_window.o 0 2>&1");
}
if (isset($_GET['updateTemp'])) {
  echo "Updating temperature";
  $output = exec("sudo /home/moham/www/c-bin/record_temp.o /home/moham/www/db/temp.db 2>&1");
  echo $output;
}
?>
<!DOCTYPE html>
<html>

<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="content-type" content="text/html; charset=windows-1252">
  <title>Automaed Window Control System</title>
  <link rel="stylesheet" type="text/css" href="style.css">
  <script defer type="text/javascript" src="script.js"></script>
</head>

<body>
  <div class="pedection-container">
    <form action="index.php" method="get">
      <button type="submit" name="updateTemp" value="1">Update temperature</button>
      <button type="submit" name="openWindow" value="1">Open window</button>
      <button type="submit" name="closeWindow" value="1">Close winodw</button>
    </form>
  </div>
  <h1>Temperature Database</h1>
  <table id="temp-table">
    <tr>
      <th style="padding-inline-end: 200px;">Time</th>
      <th>Temperature</th>
    </tr>
    <?php
    $db = new SQLite3('db/temp.db');
    $query = "SELECT * FROM temp";
    $results = $db->query($query);
    while ($row = $results->fetchArray(SQLITE3_ASSOC)) {
      echo "<tr>";
      echo "<td>" . $row['time'] . "</td>";
      echo "<td>" . $row['temp'] . "</td>";
      echo "</tr>";
    }
    $db->close();
    ?>
  </table>
</body>

</html>