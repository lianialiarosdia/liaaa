<?php 

   //Simpan dengan nama file panel.php
     require "koneksidb.php";
     $data    = query("SELECT * FROM tb_monitoring")[0];


 ?>
 
<!doctype html>
<html lang="en">
 
<head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <title>Data Tables</title>
    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="assets/vendor/bootstrap/css/bootstrap.min.css">
    <link href="assets/vendor/fonts/circular-std/style.css" rel="stylesheet">
    <link rel="stylesheet" href="assets/libs/css/style.css">
    <link rel="stylesheet" href="assets/vendor/fonts/fontawesome/css/fontawesome-all.css">
    <link rel="stylesheet" type="text/css" href="assets/vendor/datatables/css/dataTables.bootstrap4.css">
    <link rel="stylesheet" type="text/css" href="assets/vendor/datatables/css/buttons.bootstrap4.css">
    <link rel="stylesheet" type="text/css" href="assets/vendor/datatables/css/select.bootstrap4.css">
    <link rel="stylesheet" type="text/css" href="assets/vendor/datatables/css/fixedHeader.bootstrap4.css">
</head>

<body>
    
    <?php 
        session_start();
        if($_SESSION['status']!="login"){
            header("location:index.php?pesan=belum_login");
        }

        if(isset($_GET['pesan'])){
            if($_GET['pesan'] == "berhasil"){
                echo "<script type='text/javascript'>alert('Log berhasil dihapus!');</script>";
            }
        }
	?>

    <div class="dashboard-main-wrapper">
       
        <div class="dashboard-header">
            <nav class="navbar navbar-expand-lg bg-white fixed-top">
                <a class="navbar-brand" href="dashboard.php">RFID PHP</a>

            </nav>
        </div>
      
        <div class="nav-left-sidebar sidebar-dark">
            <div class="menu-list">
                <nav class="navbar navbar-expand-lg navbar-light">
                    <a class="d-xl-none d-lg-none" href="#">Dashboard</a>
                    <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
                        <span class="navbar-toggler-icon"></span>
                    </button>
                    <div class="collapse navbar-collapse" id="navbarNav">
                        <ul class="navbar-nav flex-column">
                            <li class="nav-divider">
                                Menu
								<ul class="nav flex-column">
                                    <li class="nav-item">
                                        <a class="nav-link" href="dashboard.php">Dashboard</a>
                                    </li>
                                    <li class="nav-item">
                                        <a class="nav-link" href="data-tables.php">Log Data</a>
                                    </li>
                                    <li class="nav-item">
                                        <a class="nav-link" href="inputdata.php">Tambah Data</a>
                                    </li>
                                    <li class="nav-item">
                                        <a class="nav-link" href="register.php">Tambah User</a>
                                    </li>
                                    <li class="nav-item">
                                        <a class="nav-link" href="lihatdata.php">Lihat Data</a>
                                    </li>
                                    <li class="nav-item">
                                        <a class="nav-link" href="tambahsaldo.php">Tambah Saldo</a>
                                    </li>
                                </ul>
                            </li>  
                            <li class="nav-divider">
                                Setting
								<ul class="nav flex-column">
                                    <li class="nav-item">
                                        <a class="nav-link" href="logout.php">Logout</a>
                                    </li>
                                </ul>
                            </li>  
                        </ul>
                    </div>
                </nav>
            </div>
        </div>
       
        <div class="dashboard-wrapper">
            <div class="container-fluid  dashboard-content">
               
                <div class="row">
                    <div class="col-xl-12 col-lg-12 col-md-12 col-sm-12 col-12">
                        <div class="page-header">
                            <h2 class="pageheader-title">Log Data</h2>
                            
                        </div>
                    </div>
                </div>
             
                <div class="row">
                    
                    <div class="col-xl-12 col-lg-12 col-md-12 col-sm-12 col-12">
                        <div class="card">
                            <div class="card-header">
                                <h5 class="mb-0">Data Tables - Print, Excel, CSV, PDF Buttons</h5>
                          
                            </div>
                            <div class="card-body">
                                <div class="table-responsive">
                                    <table id="example" class="table table-striped table-bordered second" style="width:100%">
                                        <thead>
                                            <tr>
                                                <th>No</th>
                                                <th>Tanggal</th>
                                                <th>Rfid</th>
                                                <th>Nama</th>
                                                <th>Alamat</th>
                                                <th>Telepon</th>
                                                <th>Saldo Awal</th>
                                                <th>Bayar</th>
                                                <th>Saldo Ahir</th>
                                                <th>Nama Tol</th>
                                            </tr>
                                        </thead>
                                        <tbody>
                                            <?php
                                            
                                                $datatampil = mysqli_query($koneksi, "SELECT * from tb_simpan inner join tb_daftarrfid on tb_simpan.rfid=tb_daftarrfid.rfid");
                                                $no=1;
                                                if (is_array($datatampil) || is_object($datatampil)){
                                                    foreach ($datatampil as $row){
                                                        echo "<tr class= bg-white >
                                                                <td>$no</td>
                                                                <td>".$row['tanggal']."</td>
                                                                <td>".$row['rfid']."</td>
                                                                <td>".$row['nama']."</td>
                                                                <td>".$row['alamat']."</td>
                                                                <td>".$row['telepon']."</td>
                                                                <td>".$row['saldo']."</td>
                                                                <td>".$row['bayar']."</td>
                                                                <td>".$row['saldoahir']."</td>
                                                                <td>".$row['namatol']."</td>
                                                            </tr>";
                                                        $no++;
                                                    }
                                                }

                                                $koneksi->close();
                                            ?>
                                        </tbody>
                                    </table>

                                    <a href="hapusdata.php" class="btn btn-danger btn-block mt-5" onclick="return confirm('Yakin akan menghapus log?')">Clear Log Data</a>

                                </div>
                            </div>

                        </div>
                    </div>
                  
                </div>
                
            </div>
           
        </div>
    </div>
   
    <!-- Optional JavaScript -->
    
    <script src="assets/vendor/jquery/jquery-3.3.1.min.js"></script>
    <script src="assets/vendor/bootstrap/js/bootstrap.bundle.js"></script>
    <script src="assets/vendor/slimscroll/jquery.slimscroll.js"></script>
    <script src="assets/vendor/multi-select/js/jquery.multi-select.js"></script>
    <script src="assets/libs/js/main-js.js"></script>
    <script src="https://cdn.datatables.net/1.10.19/js/jquery.dataTables.min.js"></script>
    <script src="assets/vendor/datatables/js/dataTables.bootstrap4.min.js"></script>
    <script src="https://cdn.datatables.net/buttons/1.5.2/js/dataTables.buttons.min.js"></script>
    <script src="assets/vendor/datatables/js/buttons.bootstrap4.min.js"></script>
    <script src="assets/vendor/datatables/js/data-table.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jszip/3.1.3/jszip.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/pdfmake/0.1.36/pdfmake.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/pdfmake/0.1.36/vfs_fonts.js"></script>
    <script src="https://cdn.datatables.net/buttons/1.5.2/js/buttons.html5.min.js"></script>
    <script src="https://cdn.datatables.net/buttons/1.5.2/js/buttons.print.min.js"></script>
    <script src="https://cdn.datatables.net/buttons/1.5.2/js/buttons.colVis.min.js"></script>
    <script src="https://cdn.datatables.net/rowgroup/1.0.4/js/dataTables.rowGroup.min.js"></script>
    <script src="https://cdn.datatables.net/select/1.2.7/js/dataTables.select.min.js"></script>
    <script src="https://cdn.datatables.net/fixedheader/3.1.5/js/dataTables.fixedHeader.min.js"></script>

</body>
 
</html>