<?php 
   
   require "koneksidb.php";

   $ambilrfid	 = $_GET["rfid"];
   $ambilnamatol	 = $_GET["namatol"];
   
   date_default_timezone_set('Asia/Jakarta');
   $tgl=date("Y-m-d G:i:s");
   
   
		//UPDATE DATA REALTIME PADA TABEL tb_monitoring
		$sql      = "UPDATE tb_monitoring SET tanggal	= '$tgl', rfid	= '$ambilrfid', namatol	= '$ambilnamatol'";
		$koneksi->query($sql);
		
   //LOGIKA Pembayaran
   $ambilsaldo = query ("SELECT saldoawal FROM tb_daftarrfid WHERE rfid = '$ambilrfid'")[0];
   $saldolama = $ambilsaldo['saldoawal'];
   
   $ambilharga = query("SELECT bayar FROM tb_tol WHERE namatol = '$ambilnamatol'")[0];
   $sqlbayar = $ambilharga['bayar'];
   
   $saldobaru = $saldolama - $sqlbayar;
		
    //INSERT DATA REALTIME PADA TABEL tb_save  	
	  
		$sqlsave = "INSERT INTO tb_simpan (tanggal, rfid, saldo, bayar, saldoahir, namatol) VALUES ('" . $tgl . "', '" . $ambilrfid . "', '" . $saldolama . "', '" . $sqlbayar . "', '" . $saldobaru . "','" . $ambilnamatol . "')";
		$koneksi->query($sqlsave);
   
      $sql      = "UPDATE tb_daftarrfid SET saldoawal='$saldobaru' WHERE rfid='$ambilrfid'";
		$koneksi->query($sql);
   

		//MENJADIKAN JSON DATA
		//$response = query("SELECT * FROM tb_monitoring")[0];
		$response = query("SELECT * FROM tb_daftarrfid,tb_monitoring WHERE tb_daftarrfid.rfid='$ambilrfid'" )[0];
    //$response = query("SELECT * FROM tb_simpan,tb_monitoring WHERE tb_simpan.rfid='$ambilrfid'" )[0];
      	$result = json_encode($response);
     	echo $result;



 ?>