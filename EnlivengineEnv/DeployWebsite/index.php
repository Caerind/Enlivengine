<?php

	$buildDir = "builds";

	class Build {
		public $project;
		public $branch;
		public $version;
		public $config;
		public $platform;
		public $path;
	}
	
	$builds = array();

	$files = scandir($buildDir);
	foreach ($files as $file) 
	{
		if ($file !== '.' && $file !== '..')
		{
			$values = explode("-", $file);
			// TODO : Check validity
		
			$b = new Build();
			$b->project = $values[0];
			$b->branch = $values[1];
			$b->config = $values[2];
			$b->version = $values[3];
			$b->platform = $values[4]; // TODO : Remove extension
			$b->path = $buildDir . '/' . $file;
		
			array_push($builds, $b);
		}
	}
?>

<!doctype html>
<html lang="en">
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <title>Enlivengine builds</title>

    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.6.3/css/all.css" integrity="sha384-UHRtZLI+pbxtHCWp1t77Bi1L4ZtiqrqD80Kn4Z8NTSRyMA2Fd33n5dQ8lWUE00s/" crossorigin="anonymous">
    <link rel="stylesheet" href="https://unpkg.com/bootstrap-table@1.18.2/dist/bootstrap-table.min.css">
  </head>
  <body>
	<h1>Enlivengine <b>builds</b></h1>
    <table 
		data-toggle="table"
		data-sort-class="table-active"
		data-sortable="true"
	>
      <thead>
        <tr>
          <th data-field="project" data-sortable="true">Project</th>
          <th data-field="branch" data-sortable="true">Branch</th>
          <th data-field="version" data-sortable="true">Version</th>
          <th data-field="platform" data-sortable="true">Platform</th>
          <th data-field="config" data-sortable="true">Config</th>
		  <th>Link</th>
        </tr>
      </thead>
      <tbody>
	  
		<?php
		foreach ($builds as $build) 
		{
			?>
			
			<tr>
				<td><?php echo $build->project; ?></td>
				<td><?php echo $build->branch; ?></td>
				<td><?php echo $build->version; ?></td>
				<td><?php echo $build->platform; ?></td>
				<td><?php echo $build->config; ?></td>
				<td><a href="<?php echo $build->path; ?>">Link</a></td>
			</tr>
			
			<?php
		}
		?>
		
      </tbody>
    </table>

    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.5.1/jquery.min.js" integrity="sha512-bLT0Qm9VnAYZDflyKcBaQ2gg0hSYNQrJ8RilYldYQ1FxQYoCLtUjuuRuZo+fjqhx/qtq/1itJ0C2ejDxltZVFg==" crossorigin="anonymous"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js" integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1" crossorigin="anonymous"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
    <script src="https://unpkg.com/bootstrap-table@1.18.2/dist/bootstrap-table.min.js"></script>
  </body>
</html>