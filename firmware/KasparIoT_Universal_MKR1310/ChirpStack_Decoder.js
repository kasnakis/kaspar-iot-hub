// ChirpStack v4. Αντιστοίχιση ID σύμφωνα με τις θέσεις στο μενού.
function decodeUplink(input) {
 const b=input.bytes, names=['temperature','humidity','sensor3','sensor4','sensor5','sensor6','sensor7','sensor8'];
 if(b[0]===123){try{return {data:JSON.parse(String.fromCharCode.apply(null,b))};}catch(e){return {errors:['Invalid JSON']};}}
 if(b.length<2||b[0]!==1||b[1]>8||b.length!==2+5*b[1])return {errors:['Invalid binary payload']};
 const data={};for(let i=2;i<b.length;i+=5){let id=b[i];if(id<1||id>8)return {errors:['Invalid sensor ID']};let x=(b[i+1]<<24)|(b[i+2]<<16)|(b[i+3]<<8)|b[i+4];data[names[id-1]]=x/100;}return {data};
}
