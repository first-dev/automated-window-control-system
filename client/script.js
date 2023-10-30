const updateTemperature = () => {
  const xmlhttp = new XMLHttpRequest()
  xmlhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      const table = document.getElementById('temp-table')
      // insert new row
      const newRow = table.insertRow(1)
      // insert new cells
      const newCell1 = newRow.insertCell(0)
      const newCell2 = newRow.insertCell(1)
      // insert new data
      newCell1.innerHTML = new Date().toLocaleString()
      newCell2.innerHTML = this.responseText
      // document.getElementById('someP').innerHTML = this.responseText
    }
  }
  xmlhttp.open('GET', 'updateTemp.php', true)
  xmlhttp.send()
}
