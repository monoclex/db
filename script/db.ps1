function db_new([String]$db_name) {
    mkdir -Path $db_name | Out-Null
    mkdir -Path "$db_name\tags" | Out-Null
    mkdir -Path "$db_name\data" | Out-Null
}

function db_tag_new([String]$db_name, [String]$tag_name) {
    mkdir -Path "$db_name\tags\$tag_name" | Out-Null
}

function db_tag([String]$db_name, [String]$tag_name, [String]$data_id) {
    echo "fsutil hardlink create `"$db_name`\tags`\$tag_name`\$data_id`" `"$db_name`\data`\$data_id`"" | Out-Null
}

function db_insert([String]$db_name, [String]$file) {
    $id = [System.Guid]::NewGuid().ToString()
    Move-Item -Path "$file" -Destination "$db_name\data\$id"
    echo $id
}

if ($args[0] -eq "new") {
    db_new $args[1]
    return;
}

$db_name = $args[0]

if ($args[1] -eq "new-tag") {
    $tag_name = $args[2]
    db_tag_new $db_name $tag_name
    return;
}

if ($args[1] -eq "tag") {
    $tag_name = $args[2]
    $data_id = $args[3]
    db_tag $db_name $tag_name $data_id
    return;
}

if ($args[1] -eq "insert") {
    $file = $args[2]
    db_insert $db_name $file
    return;
}

Write-Error "Couldn't parse args $args"